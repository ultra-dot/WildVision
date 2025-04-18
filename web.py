from flask import Flask, request, jsonify, Response, render_template
from pymongo import MongoClient
import gridfs
import datetime
from bson import ObjectId

app = Flask(__name__)

# Koneksi ke MongoDB Atlas
MONGO_URI = "your-mongodb-url"

client = MongoClient(MONGO_URI)
db = client["esp32cam_db"]
fs = gridfs.GridFS(db)

@app.route("/")
def gallery():
    """Menampilkan halaman galeri foto."""
    files = fs.find()
    file_list = [{"filename": file.filename, "file_id": str(file._id)} for file in files]
    return render_template("gallery.html", files=file_list)

@app.route("/list")
def list_photos():
    """Mengambil daftar foto terbaru."""
    files = db.fs.files.find().sort("uploadDate", -1)  # Ambil daftar file terbaru
    file_list = [{"filename": file["filename"], "file_id": str(file["_id"])} for file in files]
    return jsonify(file_list)

@app.route("/view/<file_id>")
def view_photo(file_id):
    try:
        file = fs.get(ObjectId(file_id))  # Mengambil file dari GridFS
        return Response(file.read(), mimetype="image/jpeg")  # Menampilkan gambar
    except Exception as e:
        return jsonify({"error": str(e)}), 404

@app.route("/download/<file_id>")
def download_photo(file_id):
    """Mengunduh gambar sebagai file JPG."""
    try:
        file = fs.get(file_id)
        response = Response(file.read(), mimetype="image/jpeg")
        response.headers["Content-Disposition"] = f"attachment; filename={file.filename}"
        return response
    except Exception as e:
        return jsonify({"error": str(e)}), 404

@app.route("/upload", methods=["POST"])
def upload_photo():
    try:
        if not request.data or len(request.data) == 0:
            print("ERROR: Tidak ada data dalam request")
            return jsonify({"error": "File kosong atau tidak terkirim"}), 400

        print(f"✅ Data diterima: {len(request.data)} bytes")  # Debug log

        filename = f"esp32cam_{datetime.datetime.now().strftime('%Y%m%d_%H%M%S')}.jpg"
        file_id = fs.put(request.data, filename=filename)

        return jsonify({"message": "Foto berhasil disimpan", "file_id": str(file_id)}), 200

    except Exception as e:
        print(f"❌ ERROR: {str(e)}")  # Log error
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=5000, debug=True, use_reloader=False)

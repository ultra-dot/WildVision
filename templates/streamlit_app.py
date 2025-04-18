import streamlit as st
from pymongo import MongoClient
import gridfs
from PIL import Image
from io import BytesIO
import datetime

# Koneksi ke MongoDB
MONGO_URI = "mongodb+srv://mono:R08SLgsI7AW2rfkE@innovatrix.9m9bpe9.mongodb.net/?retryWrites=true&w=majority&appName=Innovatrix"  # Ganti dengan URI MongoDB Anda
client = MongoClient(MONGO_URI)
db = client["esp32cam_db"]
fs = gridfs.GridFS(db)

# Fungsi untuk mengupload foto ke MongoDB
def upload_photo():
    try:
        # Mengupload foto
        photo_file = st.file_uploader("Pilih foto untuk diupload", type=["jpg", "jpeg", "png"])

        if photo_file is not None:
            # Baca foto dan simpan ke MongoDB GridFS
            file_data = photo_file.read()
            filename = f"esp32cam_{datetime.datetime.now().strftime('%Y%m%d_%H%M%S')}.jpg"
            file_id = fs.put(file_data, filename=filename)
            st.success(f"Foto berhasil diupload dengan ID: {file_id}")
    except Exception as e:
        st.error(f"Error saat mengupload foto: {e}")

# Fungsi untuk mengambil foto dari GridFS
def load_photos():
    try:
        # Ambil daftar file dari GridFS
        files = fs.find()
        file_list = [{"filename": file.filename, "file_id": str(file._id)} for file in files]
        return file_list
    except Exception as e:
        st.error(f"Error saat memuat foto: {e}")
        return []

# Fungsi untuk menampilkan galeri foto
def display_gallery():
    photos = load_photos()

    if not photos:
        st.write("Tidak ada foto yang tersedia.")
        return

    st.title("Galeri ESP32-CAM")
    
    # Menampilkan gambar satu per satu
    for photo in photos:
        file_id = photo['file_id']
        
        try:
            file = fs.get(file_id)
            image = Image.open(BytesIO(file.read()))
            st.image(image, caption=photo['filename'], use_column_width=True)
            
            # Menyediakan link untuk mengunduh foto
            download_url = f"/download/{file_id}"
            st.markdown(f"[Download {photo['filename']}]({download_url})", unsafe_allow_html=True)
        except gridfs.errors.NoFile:
            st.warning(f"File dengan ID {file_id} tidak ditemukan di GridFS.")
        except Exception as e:
            st.error(f"Terjadi kesalahan saat menampilkan foto: {e}")

# Fungsi utama aplikasi Streamlit
def main():
    st.sidebar.title("Pilihan")
    option = st.sidebar.selectbox("Pilih aksi", ("Upload Foto", "Lihat Galeri"))

    if option == "Upload Foto":
        upload_photo()
    elif option == "Lihat Galeri":
        display_gallery()

if __name__ == "__main__":
    main()

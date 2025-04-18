import streamlit as st
from pymongo import MongoClient
import gridfs
from PIL import Image
from io import BytesIO

# Koneksi ke MongoDB Atlas
MONGO_URI = "mongodb+srv://mono:R08SLgsI7AW2rfkE@innovatrix.9m9bpe9.mongodb.net/?retryWrites=true&w=majority&appName=Innovatrix"  # Ganti dengan URI MongoDB Anda
client = MongoClient(MONGO_URI)
db = client["esp32cam_db"]
fs = gridfs.GridFS(db)

def load_photos():
    """Memuat foto dari MongoDB."""
    files = fs.find()
    file_list = [{"filename": file.filename, "file_id": str(file._id)} for file in files]
    return file_list

def display_gallery():
    """Menampilkan galeri foto di Streamlit."""
    photos = load_photos()
    if not photos:
        st.write("Tidak ada foto yang tersedia.")
        return

    st.title("Galeri ESP32-CAM")
    for photo in photos:
        # Menampilkan gambar
        file_id = photo['file_id']
        file = fs.get(file_id)
        image = Image.open(BytesIO(file.read()))

        st.image(image, caption=photo['filename'], use_column_width=True)

        # Link untuk mengunduh foto
        download_url = f"/download/{file_id}"
        st.markdown(f"[Download {photo['filename']}]({download_url})", unsafe_allow_html=True)

def main():
    """Main function untuk Streamlit."""
    display_gallery()

if __name__ == "__main__":
    main()

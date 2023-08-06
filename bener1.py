import serial
import cv2
import time
import sys

# Inisialisasi koneksi serial dengan Arduino
ser = serial.Serial('COM6', 9600)  # Ganti 'COMx' dengan nama port serial Arduino

# Fungsi untuk menyalakan kamera dan melakukan deteksi wajah
def start_deteksi():
    camera = cv2.VideoCapture(0)  # Ganti 0 dengan nomor kamera jika Anda memiliki beberapa kamera.

    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')

    detected_faces = 0
    timer_duration = 1  # Ubah nilai ini menjadi detik (bukan milidetik)
    start_time = cv2.getTickCount()  # Pindahkan inisialisasi timer ke luar loop

    while True:
        ret, frame = camera.read()
        if not ret:
            break  # Keluar dari loop jika tidak bisa mendapatkan frame dari kamera

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, scaleFactor=1.3, minNeighbors=5, minSize=(30, 30))

        detected_faces = len(faces)
        for (x, y, w, h) in faces:
            cv2.rectangle(frame, (x, y), (x+w, y+h), (0, 255, 0), 2)  # Gambar rectangle (hijau) di sekitar wajah

        if detected_faces >= 2:
            camera.release()
            cv2.destroyAllWindows()
            ser.write(b'matikan LED\n')
            print("Wajah terdeteksi, iterasi selesai")
            break

        current_time = cv2.getTickCount()
        elapsed_time = (current_time - start_time) / cv2.getTickFrequency()  # Ubah ke detik

        if elapsed_time >= timer_duration:
            if detected_faces < 2:
                time.sleep(1) 
            ser.write(b'matikan LED\n')
            print("Iterasi maksimum tercapai")
            break

        cv2.imshow('Face Detection', frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    camera.release()
    cv2.destroyAllWindows()

try:
    is_motion_detected = False

    while True:
        data = ser.readline().decode('utf-8').rstrip()

        if data == "kuning menyala" and not is_motion_detected:
            is_motion_detected = True
            start_deteksi()
            # sys.exit(0)
        elif data == "merah menyala":
            is_motion_detected = False  # Setelah deteksi selesai, kembali ke kondisi awal
            ser.write(b'hijau menyala\n')

except KeyboardInterrupt:
    ser.close()


# programa_a.py
import cv2
import numpy as np
from multiprocessing import shared_memory, Event
from ultralytics import YOLO
import multiprocessing as mp
import time

# YOLO model
model = YOLO("../modelos/SHARK_SAWFISH.pt")

# Configurações de frame
WIDTH, HEIGHT, CHANNELS = 640, 480, 3
FRAME_SIZE = WIDTH * HEIGHT * CHANNELS

def main():
    # Cria memória compartilhada e evento
    shm = shared_memory.SharedMemory(create=True, size=FRAME_SIZE, name='frame_shm')
    event = Event()
    event_shm = shared_memory.ShareableList([False], name='event_flag')

    # Abre câmera
    cap = cv2.VideoCapture(0)

    try:
        while True:
            ret, frame = cap.read()
            if not ret:
                continue

            frame = cv2.resize(frame, (WIDTH, HEIGHT))
            np_frame = np.ndarray((HEIGHT, WIDTH, CHANNELS), dtype=np.uint8, buffer=shm.buf)

            # Processa com YOLO e exibe (opcional)
            results = model(frame, show=True, conf=0.5, imgsz=640)

            # Escreve o frame na memória
            np.copyto(np_frame, frame)

            # Sinaliza que frame está pronto
            event_shm[0] = True

            # Pequeno delay para evitar uso excessivo da CPU
            time.sleep(0.01)

    except KeyboardInterrupt:
        print("Programa A encerrado.")

    finally:
        cap.release()
        shm.close()
        shm.unlink()
        event_shm.shm.close()
        event_shm.shm.unlink()

if __name__ == "__main__":
    main()

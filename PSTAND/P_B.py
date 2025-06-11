# programa_b.py
import cv2
import numpy as np
from multiprocessing import shared_memory
from ultralytics import YOLO
import time

# YOLO model
model = YOLO("../modelos/yolo11n.pt")

# Configurações de frame
WIDTH, HEIGHT, CHANNELS = 640, 480, 3
FRAME_SIZE = WIDTH * HEIGHT * CHANNELS

def main():
    # Conecta à memória compartilhada
    shm = shared_memory.SharedMemory(name='frame_shm')
    np_frame = np.ndarray((HEIGHT, WIDTH, CHANNELS), dtype=np.uint8, buffer=shm.buf)
    event_shm = shared_memory.ShareableList(name='event_flag')

    qnto_frames = 0

    try:
        while True:
            # Espera até que o produtor sinalize novo frame
            if event_shm[0]:
                # Copia o frame para evitar sobreposição de leitura
                frame_copy = np_frame.copy()
                results = model(frame_copy, show=True, conf=0.5, imgsz=640)

                qnto_frames += 1
                print(f"Frames processados: {qnto_frames}")

                # Reseta sinal
                event_shm[0] = False
            else:
                time.sleep(0.01)  # Evita busy-wait

    except KeyboardInterrupt:
        print("Programa B encerrado.")

    finally:
        shm.close()
        event_shm.shm.close()
        cv2.destroyAllWindows()

if __name__ == "__main__":
    main()

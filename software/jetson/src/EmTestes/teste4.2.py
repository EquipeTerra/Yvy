# programa_b.py
import cv2
import numpy as np
from multiprocessing import shared_memory
from ultralytics import YOLO
model = YOLO("../modelos/yolo11n.pt")
# Configurações
WIDTH = 640
HEIGHT = 480
CHANNELS = 3
FRAME_SIZE = WIDTH * HEIGHT * CHANNELS

# Conecta na memória compartilhada
qnto_frames = 0
shm = shared_memory.SharedMemory(name='frame_shm')
try:
    while True:
        # Lê da memória compartilhada
        np_frame = np.ndarray((HEIGHT, WIDTH, CHANNELS), dtype=np.uint8, buffer=shm.buf)
        results = model(np_frame, show=True, conf=0.5,imgsz=640)
        qnto_frames += 1
        print(f"Frames processados: {qnto_frames}")


except KeyboardInterrupt:
    print("Programa B encerrado.")

finally:
    shm.close()
    cv2.destroyAllWindows()

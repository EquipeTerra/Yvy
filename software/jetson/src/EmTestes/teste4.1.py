# programa_a.py
import cv2
import numpy as np
from multiprocessing import shared_memory
import time

# Configurações
WIDTH = 640
HEIGHT = 480
CHANNELS = 3
FRAME_SIZE = WIDTH * HEIGHT * CHANNELS

# Cria a memória compartilhada
shm = shared_memory.SharedMemory(create=True, size=FRAME_SIZE, name='frame_shm')

# Captura da câmera
cap = cv2.VideoCapture(0)

try:
    while True:
        ret, frame = cap.read()
        if not ret:
            continue

        # Redimensiona para o tamanho padrão
        frame = cv2.resize(frame, (WIDTH, HEIGHT))
        # Escreve na memória compartilhada
        np_frame = np.ndarray((HEIGHT, WIDTH, CHANNELS), dtype=np.uint8, buffer=shm.buf)
        np.copyto(np_frame, frame)
        cv2.imshow('Frame Compartilhado1', np_frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

except KeyboardInterrupt:
    print("Programa A encerrado.")

finally:
    cap.release()
    shm.close()
    shm.unlink()  # Libera a memória compartilhada

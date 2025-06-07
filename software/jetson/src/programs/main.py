"""
Esse arquivo tem como objetivo principal ser um arquivo leve pra rodar a yolo com modelo treinado.
"""

"""
VALORES DE DETECCAO
0: SAWFISH
1: SHARK
"""

from ultralytics import YOLO
from socket import socket, AF_INET, SOCK_STREAM
from json import dumps
import Jetson.GPIO as GPIO


class IA:
    def __init__(self):
        self.state_dict = {
            0: [0, 1],  # Detectar SHARK e SAWFISH no estado 0
            1: [1]      # Detectar só SHARK no estado 1
            # Se quiser mais estados, só adicionar aqui
        }
        self.HOST = '192.168.0.2'
        self.PORT = 65432
        self.model = YOLO('../modelos/SHARK_SAWFISH.pt')
        self.state = 0
        self.pins = [11, 12, 13, 15]  # Pinos FÍSICOS!!!
        self.gpio_setup()

    def gpio_setup(self):
        GPIO.setmode(GPIO.BOARD)
        for pin in self.pins:
            GPIO.setup(pin, GPIO.IN)

    def StateLen(self):
        state = 0
        mult = 1
        for pin in self.pins:
            if GPIO.input(pin):
                state += mult
            mult *= 2
        if state != self.state:
            print(f"Estado mudou para {state}")
        self.state = state
        return state

    def SendDictionary(self, dicio):
        try:
            with socket(AF_INET, SOCK_STREAM) as s:
                s.connect((self.HOST, self.PORT))
                data_json = dumps(dicio) + '\n'
                s.sendall(data_json.encode('utf-8'))
        except ConnectionAbortedError as e:
            print(f"Conexão abortada: {e}")
        except Exception as e:
            print(f"Erro inesperado: {e}")

    def run(self):
        print("Iniciando detecção...")
        for results in self.model.predict(
                source='0', 
                verbose=False, 
                stream=True, 
                show=False, 
                imgsz=640, 
                save=True, 
                max_det=1, 
                conf=0.1):

            self.StateLen()  # Atualiza o estado a cada frame

            for result in results:
                boxes = result.boxes.data.cpu().numpy().tolist()
                filtered_data = []

                for box in boxes:
                    class_id = int(box[5])
                    if class_id in self.state_dict.get(self.state, []):
                        filtered_data.append(box[:5] + [class_id])

                if filtered_data:
                    dicio = {
                        'data': filtered_data,
                        'names': result.names,
                        'cam': 0
                    }
                    self.SendDictionary(dicio)


if __name__ == '__main__':
    ia = IA()
    try:
        ia.run()
    except KeyboardInterrupt:
        GPIO.cleanup()
        print("Programa encerrado com segurança.")

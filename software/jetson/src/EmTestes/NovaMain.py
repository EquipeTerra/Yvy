"""
Esse arquivo tem como objetivo principal ser um arquivo leve pra rodar a yolo com modelo treinado.
"""
"""
Oque eu tenho que fazer:
-Fazer um programa que capture a imagem com memoria compartilhada
-Fazer um programa que use da memoria compartilhada para rodar a yolo
-Fazer um programa que use da memoria compartilhada para detectar os retangulos do gate.

"""



"""
VALORES DE DETECCAO
0: SAWFISH
1: SHARK
"""
"""
ESTADOS
0: Detectar ZERADO
1: Detectar TUDO
2: Detectar SHARK e SAWFISH
3: Detectar SHARK


"""





from ultralytics import YOLO
from socket import socket, AF_INET, SOCK_STREAM
from json import dumps
import Jetson.GPIO as GPIO

class IA:
    def __init__(self):
        self.state_dict = {
            0: [], #NONE
            1: [0,1], #ALL
            2: [0, 1], 
            3: [1],
            4: [0]       
            # Se quiser mais estados, só adiciona aqui
        }
        self.HOST = '192.168.0.1'
        self.PORT = 65432
        self.model = YOLO('../modelos/SHARK_SAWFISH.pt')
        self.state = 0
        self.pins = [11, 12, 13, 15]  # Pinos FISICOS!!!
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
        self.state = state

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
        while True:
            self.StateLen()
            classes = self.state_dict.get(self.state)

            if classes is None:
                print(f"Estado {self.state} não mapeado, pulando detecção...")
                continue

            for results in self.model.predict(
                source='0',
                verbose=False,
                stream=True,
                show=True,
                imgsz=640,
                save=False,
                max_det=1,
                conf=0.8,
                classes=classes
            ):
                for result in results:
                    boxes = result.boxes.data.cpu().numpy().tolist()
                    dicio = {
                        'data': [box[:5] + [int(box[5])] for box in boxes],  # Corrige class_id para int
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

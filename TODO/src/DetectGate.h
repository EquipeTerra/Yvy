# 
# Implementação em PYTHON
#

#
# @brief Pré-processa a imagem para uma melhor detecção do gate.
# @details Aplica filtro Gaussiano, melhoria de contraste com CLAHE, 
#          converte a imagem de BGR para HSV, transforma para escala de cinza e binariza.
# @param[in] image OpenCV Mat com a imagem a ser processada.
# @return Imagem pré-processada.
#
def PreProcessImage(image):
    return image

#
# @brief Recebe uma imagem já binarizada, encontra os contornos e aproxima valores retangulares.
# @details Utiliza kernel do OpenCV para detectar linhas verticais formando retângulos. 
#          Limita a detecção a 2 retângulos correspondentes às barras laterais.
#          Calcula as coordenadas com base no centro da imagem.
#          Esta função será executada em paralelo com a YOLO.
# @param[in] image OpenCV Mat já binarizada.
# @return coordenadas: lista com as coordenadas dos retângulos encontrados.
#
def DetectGate(image):
    return coordenadas

#
# Notas:
# - O OpenCV fornece a maioria das funções necessárias para a implementação.
# - A função de pré-processamento pode ser chamada antes ou dentro da detecção do gate.
#   Se for chamada dentro, a imagem de entrada não deve estar binarizada.
# - É necessário que a imagem esteja em escala de cinza e binarizada para a detecção do gate.
#

#ifndef CAPTURE_H
#define CAPTURE_H

#include <opencv2/opencv.hpp>

class Capture {
public:
    /**
     * @brief Construtor que inicializa a captura com o índice da câmera.
     * @param cam_index Índice da câmera (0 por padrão).
     */
    Capture(int cam_index = 0);

    /**
     * @brief Destrutor que libera a câmera.
     */
    ~Capture();

    /**
     * @brief Verifica se a câmera foi aberta com sucesso.
     */
    bool isOpened() const;

    /**
     * @brief Captura um frame da câmera.
     * @param output Frame capturado.
     * @return true se a captura foi bem-sucedida, false caso contrário.
     */
    bool getFrame(cv::Mat& output);

private:
    cv::VideoCapture cap;
};

#endif // CAPTURE_H

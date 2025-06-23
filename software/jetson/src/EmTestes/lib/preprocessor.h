// ===============================
//  Preprocessor.h
// ===============================
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

// ---- Valores de pré‑processamento ----
#define PREPROC_TARGET_WIDTH   224
#define PREPROC_TARGET_HEIGHT  224

// Normalização (valores típicos de ImageNet, mas sinta‑se livre para ajustar)
#define PREPROC_MEAN_R 0.485f
#define PREPROC_MEAN_G 0.456f
#define PREPROC_MEAN_B 0.406f

#define PREPROC_STD_R  0.229f
#define PREPROC_STD_G  0.224f
#define PREPROC_STD_B  0.225f

#include <opencv2/core.hpp>
#include <vector>

class Preprocessor {
public:
    /**
     * @brief Função principal que encadeia todas as etapas de pré‑processamento.
     * @param input Imagem de entrada (BGR ou RGB).
     * @return Imagem já redimensionada, convertida para float32, normalizada e pronta para o modelo.
     */
    cv::Mat preprocess(const cv::Mat& input);

private:
    cv::Mat resize(const cv::Mat& input);
    cv::Mat toFloat(const cv::Mat& input);
    cv::Mat normalize(const cv::Mat& input);
};

#endif 
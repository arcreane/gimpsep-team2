#pragma once

#include <opencv2/opencv.hpp>
#include <string>

// Nom de la fen�tre principale
static const std::string MAIN_WINDOW_NAME = "Interface";

/// Affiche directement dans la fen�tre principale sans en cr�er une nouvelle.
inline void WindowManagement(const std::string& /*windowTitle*/, const cv::Mat& img) {
    // On se base toujours sur MAIN_WINDOW_NAME
    cv::imshow(MAIN_WINDOW_NAME, img);

    // On laisse un tout petit d�lai pour que l'image s'affiche,
    // mais sans bloquer (0 ms). La boucle d'affichage d'Interface
    // continue de tourner.
    cv::waitKey(1);
}

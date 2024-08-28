#include "customlabel.h"

CustomLabel::CustomLabel(QWidget *parent) : QLabel(parent) {}

void CustomLabel::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        emit clicked();  // Emit the clicked signal
    }
    QLabel::mousePressEvent(event); // Call the base class implementation
}

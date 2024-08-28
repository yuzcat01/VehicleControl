#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H

#include <QLabel>
#include <QMouseEvent>

class CustomLabel : public QLabel {
    Q_OBJECT

public:
    explicit CustomLabel(QWidget *parent = nullptr);

signals:
    void clicked();  // Signal to emit when the label is clicked

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // CUSTOMLABEL_H

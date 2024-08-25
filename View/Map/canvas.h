#ifndef CANVAS_H
#define CANVAS_H

#include <QWidget>
#include <QPainter>

namespace Ui {
class Canvas;
}

struct Node {
    double lat;
    double lon;
};

class Canvas : public QWidget
{
    Q_OBJECT

public:
    Canvas();
    ~Canvas();
    //解析osm数据
    void loadOSMData(const QString &fileName);
    //绘制路线
    void drawRoadNetwork();
    //维度转换为像素坐标
    QPoint convertLatLonToPixel(double lat, double lon);

    void wgs84ToBd09(double wgLat, double wgLon, double& bdLat, double& bdLon);

    //控制好地图的精度  高清或低清[3-18]
    int m_zoom=15;
    //经度
    double m_x;
    //纬度
    double m_y;


protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::Canvas *ui;
    QMap<qint64, Node> nodes; // 用于存储节点
    QList<QList<qint64>> ways;

};

#endif // CANVAS_H

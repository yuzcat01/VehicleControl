#include "canvas.h"
#include "ui_canvas.h"
#include <QFile>
#include<QXmlStreamReader>

Canvas::Canvas() :
    QWidget(nullptr),
    ui(new Ui::Canvas),
    m_x(0),
    m_y(0),
    nodes(),
    ways()

{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
}

Canvas::~Canvas()
{
    delete ui;
}

void Canvas::paintEvent(QPaintEvent *event) {
    loadOSMData(":/qss/prefix2/View/Map/map.osm");
}

void Canvas::loadOSMData(const QString &fileName) {
    QFile file(":/qss/prefix2/View/Map/map.osm");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Unable to open file" << fileName;
        return;
    }
    QXmlStreamReader xml(&file);

    while (!xml.atEnd() && !xml.hasError()){
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement){
            if (xml.name() == QString("node")) {
                qint64 id = xml.attributes().value("id").toLongLong();
                double lat = xml.attributes().value("lat").toDouble();
                double lon = xml.attributes().value("lon").toDouble();
                nodes[id] = { lat, lon };
            }else if (xml.name() == QString("way")) {
                QList<qint64> way;
                while (!(xml.tokenType() == QXmlStreamReader::EndElement && xml.name() == QString("way"))) {
                    if (xml.tokenType() == QXmlStreamReader::StartElement && xml.name() == QString("nd")) {
                        qint64 ref = xml.attributes().value("ref").toLongLong();
                        way.append(ref);
                    }
                    xml.readNext();
                }
                ways.append(way);
            }
        }
    }

    if (xml.hasError()) {
        qDebug() << "XML error:" << xml.errorString();
    }

    file.close();
    drawRoadNetwork();
}

void Canvas::drawRoadNetwork(){
    QPainter painter(this);


    //设置画笔的宽度与颜色
    painter.setPen(QPen(Qt::red, 1));
    for (const QList<qint64> &way : ways) {
        for (int i = 0; i < way.size() - 1; ++i) {
            qint64 startId = way[i];
            qint64 endId = way[i + 1];

            if (nodes.contains(startId) && nodes.contains(endId)) {
                Node startNode = nodes[startId];
                Node endNode = nodes[endId];

                // 计算点在地图上的位置，这里假设你有一个函数将经纬度转换为像素坐标
                QPoint startPoint = convertLatLonToPixel(startNode.lat, startNode.lon);
                QPoint endPoint = convertLatLonToPixel(endNode.lat, endNode.lon);
                //qDebug() << "Drawing line from" << startPoint << "to" << endPoint;    //debug
                //startPoint.setX(500);
                //startPoint.setY(234);
                //endPoint.setX(1000);
                //endPoint.setY(468);
                // 绘制线段
                painter.drawLine(startPoint, endPoint);
            }
        }
    }

    painter.end();
}

void Canvas::wgs84ToBd09(double wgLat, double wgLon, double& bdLat, double& bdLon) {
    double x = wgLon;
    double y = wgLat;
    double z = sqrt(x * x + y * y) + 0.00002 * sin(y * M_PI);
    double theta = atan2(y, x) + 0.000003 * cos(x * M_PI);
    bdLon = z * cos(theta) + 0.0065;
    bdLat = z * sin(theta) + 0.006;
}


QPoint Canvas::convertLatLonToPixel(double lat, double lon) {
    double dx;
    double dy;


    dx=((0.471*(lon-116.185-0.0065)/(0.000002*(19-15)*(19-15))))*pow(2,m_zoom-15);
    dy=((0.580*(lat-39.7385-0.006)/(0.000002*(19-15)*(19-15))))*pow(2,m_zoom-15);

    int pixelX = (783 + dx)-m_x;
    int pixelY = (-10  - dy)-m_y;
    return QPoint(pixelX, pixelY);


}


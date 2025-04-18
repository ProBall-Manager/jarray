#ifndef HOLOGRAMBAR_H
#define HOLOGRAMBAR_H
#include <QGraphicsRectItem>
#include <QObject>
#include <QPainter>
#include <QLinearGradient>
#include <QPropertyAnimation>

class HologramBar : public QObject, public QGraphicsRectItem {
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity) // Opacity property
    Q_PROPERTY(qreal scale READ scale WRITE setScale) // Add scale property
public:
    HologramBar(const QString &label, double value, int count, QGraphicsItem *parent = nullptr);
    ~HologramBar();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    // Opacity property
    qreal opacity() const { return m_opacity; }
    void setOpacity(qreal opacity) {
        m_opacity = opacity;
        update();
    }

    // Scale property (this just uses the existing QGraphicsItem::scale functions)
    qreal scale() const { return QGraphicsItem::scale(); }
    void setScale(qreal scale) {
        QGraphicsItem::setScale(scale);
        update();
    }

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

private:
    QString m_label;
    double m_value;
    int m_count;
    QLinearGradient *m_gradient;
    QPropertyAnimation *m_glowAnimation;
    qreal m_opacity; // Variable to hold opacity value
};
#endif // HOLOGRAMBAR_H

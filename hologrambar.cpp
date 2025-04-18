#include "hologrambar.h"
#include <QGraphicsSceneHoverEvent>
#include <QTimer>
#include <QGraphicsDropShadowEffect>

// HologramBar implementation
HologramBar::HologramBar(const QString &label, double value, int count, QGraphicsItem *parent)
    : QObject(nullptr), QGraphicsRectItem(parent), m_label(label), m_value(value), m_count(count), m_opacity(1.0) {
    // Set up the hologram appearance
    setPen(Qt::NoPen);
    // Create holographic effect with gradient
    m_gradient = new QLinearGradient(0, 0, 0, 100);
    m_gradient->setColorAt(0.0, QColor(0, 200, 255, 180));
    m_gradient->setColorAt(0.5, QColor(0, 150, 255, 150));
    m_gradient->setColorAt(1.0, QColor(0, 100, 200, 180));
    // Enable hover events
    setAcceptHoverEvents(true);
    // Initialize animation
    m_glowAnimation = new QPropertyAnimation(this, "opacity");
    m_glowAnimation->setDuration(1500);
    m_glowAnimation->setStartValue(0.7);
    m_glowAnimation->setEndValue(1.0);
    m_glowAnimation->setLoopCount(-1); // Infinite loop
    m_glowAnimation->setEasingCurve(QEasingCurve::InOutSine);
    m_glowAnimation->start();
}

HologramBar::~HologramBar() {
    delete m_gradient;
    delete m_glowAnimation;
}

QRectF HologramBar::boundingRect() const {
    return rect();
}

void HologramBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option); // Mark unused parameter
    Q_UNUSED(widget); // Mark unused parameter
    painter->setRenderHint(QPainter::Antialiasing);

    // Set the opacity for the painter
    painter->setOpacity(m_opacity); // Use the opacity value

    // Get the rect for drawing
    QRectF r = rect();

    // Check if we should draw as a circle (width and height similar)
    bool isCircular = qAbs(r.width() - r.height()) < 10;

    // Draw the bar with gradient as a circle or rounded rectangle
    painter->setBrush(QBrush(*m_gradient));

    if (isCircular) {
        // Draw as a circle
        painter->drawEllipse(r);

        // Draw glowing border
        QPen glowPen(QColor(0, 220, 255, 200), 2);
        painter->setPen(glowPen);
        painter->drawEllipse(r);
    } else {
        // Draw as rounded rectangle for backward compatibility
        painter->drawRoundedRect(r, 20, 20);

        // Draw glowing border
        QPen glowPen(QColor(0, 220, 255, 200), 2);
        painter->setPen(glowPen);
        painter->drawRoundedRect(r, 20, 20);
    }

    // Draw the label
    painter->setPen(Qt::white);
    QFont font = painter->font();
    font.setBold(true);
    painter->setFont(font);

    // Draw match type at the bottom
    painter->drawText(r.adjusted(5, 5, -5, -30), Qt::AlignBottom | Qt::AlignHCenter, m_label);

    // Draw attendance value
    font.setPointSize(12);
    painter->setFont(font);
    painter->drawText(r.adjusted(5, 5, -5, -60), Qt::AlignCenter,
                      QString::number(m_value, 'f', 2));

    // Draw match count
    font.setPointSize(9);
    painter->setFont(font);
    painter->drawText(r.adjusted(5, 40, -5, -5), Qt::AlignCenter,
                      QString("Count: %1").arg(m_count));
}

void HologramBar::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    // Create glow effect when hovering
    QGraphicsDropShadowEffect *glowEffect = new QGraphicsDropShadowEffect();
    glowEffect->setColor(QColor(0, 200, 255, 255));
    glowEffect->setBlurRadius(20);
    glowEffect->setOffset(0);
    setGraphicsEffect(glowEffect);

    // Now we can animate the scale property correctly
    QPropertyAnimation *scaleAnimation = new QPropertyAnimation(this, "scale");
    scaleAnimation->setDuration(300);
    scaleAnimation->setStartValue(1.0);
    scaleAnimation->setEndValue(1.05);
    scaleAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    QGraphicsRectItem::hoverEnterEvent(event);
}

void HologramBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    // Remove glow effect when not hovering
    QPropertyAnimation *scaleAnimation = new QPropertyAnimation(this, "scale");
    scaleAnimation->setDuration(300);
    scaleAnimation->setStartValue(1.05);
    scaleAnimation->setEndValue(1.0);
    scaleAnimation->start(QAbstractAnimation::DeleteWhenStopped);

    // Delay the removal of the glow effect
    QTimer::singleShot(300, this, [this]() {
        setGraphicsEffect(nullptr);
    });

    QGraphicsRectItem::hoverLeaveEvent(event);
}

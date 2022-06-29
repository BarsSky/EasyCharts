#ifndef SKYCHART_BASE_H
#define SKYCHART_BASE_H

#include "QGraphicsView"
#include <QtCharts>
#include <vector>
#include <list>

struct Base2DChart: public QWidget{
public:
    //создаем график и заполняем его данными
    Base2DChart(QWidget* parent = nullptr){
        Q_UNUSED(parent);
        m_chart = new QChart();
        m_chart->setZValue(1);
        m_chart->legend()->setEnabled(false);
        m_chart->legend()->setVisible(false);
    }

    ~Base2DChart(){
        m_LinesNames.clear();
        m_colorsVector.clear();
        delete m_lineSeries;
        delete axisX;
        delete axisY;
        delete m_chart;
    }

    void setPoints(std::vector<float> *_x, std::vector<float> *_y){
        m_lineSeries = new QLineSeries(m_chart);
        enableOpenGl();
        for (unsigned int i = 0; i < _x->size(); i++)
            m_chartPoints.append(QPointF(_x->at(i), _y->at(i)));
        m_lineSeries->append(m_chartPoints);
        m_chart->addSeries(m_lineSeries);
        m_lineSeries->setColor(QColor(sequenceColor()));
        m_lineSeries->setName("line");
        minMaxFunc(reinterpret_cast<QLineSeries*>(m_chart->series().back())->points());
        addAsixsToChart();
        dynamicGrid();
        axisPreference();
        disableOpenGl();
    }

    QChart* getChart(){
        return m_chart;
    }
    //work with title
    QString getTitleName(){
        return m_chart->title();
    }

    void setTitleName(QString _title){
        m_chart->setTitle(_title);
    }

    void setTitleColor(QColor _color){
        m_chart->setTitleBrush(QBrush(_color));
    }

    void setTitleFont(QFont _font){
        m_chart->setTitleFont(_font);
    }
    //Work with axis
    void addAsixsToChart()
    {
        removeAxisDekart();
        //формируем новые оси
        axisX = new QValueAxis(m_chart);
        axisY = new QValueAxis(m_chart);
        //добавляем на график
        m_chart->addAxis(axisX, Qt::AlignBottom);
        m_chart->addAxis(axisY, Qt::AlignLeft);
        //задаём диапазон значений на осях графика
        axisY->setRange(m_axisMinY, m_axisMaxY);
        axisX->setRange(m_axisMinX, m_axisMaxX);
        m_lineSeries->attachAxis(axisX);
        m_lineSeries->attachAxis(axisY);
    }

    void removeAxisDekart()
    {
        if(axisX!=nullptr && axisY != nullptr){
            //clear axies
            m_chart->removeAxis(axisX);
            m_chart->removeAxis(axisY);
        }
    }

    void axisPreference(){
        //общие настройки осей
        axisX->setLabelsFont(fontType);
        axisX->setTitleFont(fontType);
        axisX->setTitleText(m_axisXName);
        axisX->setMinorTickCount(m_verticalInterval);
        //X->applyNiceNumbers();//функция красивых номеров по умолчанию от qt
        //X->setLabelsEditable(true);//меняем значения по своему умотрению, работает только для одного графика
        axisY->setLabelsFont(fontType);
        axisY->setTitleFont(fontType);
        axisY->setTitleText(m_axisYName);
        axisY->setMinorTickCount(m_horizontalInterval);
        //Y->applyNiceNumbers();
        //Y->setLabelsEditable(true);
#if (QT_VERSION >= QT_VERSION_CHECK(5,14,0))
        //по вертикальной оси
        if (!statDynamFlagV)
        {
            axisX->setLabelsVisible(true);
            axisX->setTickInterval(tickIntervalV);
            axisX->setTickType(QValueAxis::TicksDynamic);

        }
        else {
            axisX->setTickType(QValueAxis::TicksFixed);
            axisX->setTickCount(m_gridVStep);
            axisX->setLabelFormat("%." + QString::number(m_axisStepX) + "f");
        }

        //по горизонтальной оси
        if (!statDynamFlagH) {
            axisY->setLabelsVisible(true);
            axisY->setTickInterval(tickIntervalH);
            axisY->setTickType(QValueAxis::TicksDynamic);
        }
        else {
            axisY->setTickType(QValueAxis::TicksFixed);
            axisY->setTickCount(m_gridHStep);
            axisY->setLabelFormat("%." + QString::number(m_axisStepY) + "f");
        }
#else
        axisX->setTickCount(m_gridVStep);
        axisX->setLabelFormat("%." + QString::number(m_axisStepX) + "f");

        axisY->setTickCount(m_gridHStep);
        axisY->setLabelFormat("%." + QString::number(m_axisStepY) + "f");
#endif
    }

    void updateAxis(QPointF _min, QPointF _max){
        QList<QAbstractAxis*> _axes = m_chart->axes(Qt::Vertical);
        for(auto ax : _axes)
            ax->setRange(_min.y(), _max.y());
        _axes = m_chart->axes(Qt::Horizontal);
        for(auto ax : _axes)
            ax->setRange(_min.x(), _max.x());
    }
    //Special function

    void disableOpenGl(){
        for (int i = 0; i < m_chart->series().count(); i++)
        {
            m_chart->series().at(i)->setUseOpenGL(false);
            m_chart->series().at(i)->useOpenGLChanged();
        }
    }

    void enableOpenGl(){
        for (int i = 0; i < m_chart->series().count(); i++)
        {
            m_chart->series().at(i)->setUseOpenGL(true);
            m_chart->series().at(i)->useOpenGLChanged();
        }
    }

    void dynamicGrid(){
        double step;//число возводимое в степень
        double value;
        int gridNum = 5;
        //
        value = m_axisMaxX - m_axisMinX;
        step = value / gridNum;//установим шаг сетки в 10
        //переводим в формат со степенью
        QString str = QString("%1").arg(step, 0, 'E', 3);
        QString strVal = str.left(str.indexOf('E'));
        QString strExp = str.right(str.indexOf('E') - 2);
        step = strVal.toDouble();
        step = 5 * std::round(step / 5);
        if (step == 0)
            step = 1;
        tickIntervalV = step * pow(10, strExp.toInt());
        //
        value = m_axisMaxY - m_axisMinY;
        step = value / gridNum;//установим шаг сетки в 10
        //переводим в формат со степенью
        str = QString("%1").arg(step, 0, 'E', 3);
        strVal = str.left(str.indexOf('E'));
        strExp = str.right(str.indexOf('E') - 2);
        step = strVal.toDouble();
        step = 5 * std::round(step / 5);
        if (step == 0)
            step = 1;
        tickIntervalH = step * pow(10, strExp.toInt());

    }

    void minMaxFunc(QList<QPointF> _points)
    {
        std::vector<float> x,y;
        for(auto point :_points)
        {
            x.emplace_back(point.x());
            y.emplace_back(point.y());
        }
        //функция расчета минимума максимума для крайних значений осей
        auto minmaxX = std::minmax_element(x.begin(), x.end());
        auto minmaxY = std::minmax_element(y.begin(), y.end());

        double fXMin = *minmaxX.first;
        double fXMax = *minmaxX.second;
        double fYMin = *minmaxY.first;
        double fYMax = *minmaxY.second;

        if (fXMax > m_axisMaxX)
            m_axisMaxX = fXMax;

        if (fXMin < m_axisMinX)
            m_axisMinX = fXMin;

        if (fYMax > m_axisMaxY)
            m_axisMaxY = fYMax;

        if (fYMin < m_axisMinY)
            m_axisMinY = fYMin;

        if (m_axisMinY == m_axisMaxY)
        {
            m_axisMinY -= 0.1;
            m_axisMaxY += 0.1;
        }
    }

    //Work with color
    void setColorSequence(std::vector<QString> _sequnce){
        m_colorsVector = _sequnce;
    }

    std::vector<QString> getColorSequence(){
        return m_colorsVector;
    }

    QColor sequenceColor()
    {
        //последовательный цвет для графиков
        QColor color;
        if(m_colorsVector.size() == 0){
            m_colorsVector.emplace_back("black");
            m_colorsVector.emplace_back("darkgreen");
            m_colorsVector.emplace_back("midnightblue");
            m_colorsVector.emplace_back("maroon");
            m_colorsVector.emplace_back("purple");
            m_colorsVector.emplace_back("darkcyan");
            m_colorsVector.emplace_back("red");
            m_colorsVector.emplace_back("darkorange");
            m_colorsVector.emplace_back("limegreen");
            m_colorsVector.emplace_back("lightslategray");
            m_colorsVector.emplace_back("goldenrod");
        }
        //int randNum = qrand() % 10;
        color = QColor(m_colorsVector.at(countColors));
        if (countColors < m_colorsVector.size())
            countColors++;
        else
            countColors = 0;
        return color;//qrand() % 255, qrand() % 255, qrand() % 255
    }
    //work with SeriesLines
    QLineSeries* getCurrentLine(unsigned int _lineNumber){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            return reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber));
        return nullptr;
    }

    size_t getLinesCount(){
        return m_chart->series().count();
    }

    void setCurrentLineName(unsigned int _lineNumber, QString _name){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            m_chart->series().at(_lineNumber)->setName(_name);
    }

    void setLineName(QString _name){
        m_chart->series().back()->setName(_name);
    }

    void removeLine(unsigned int _lineNumber){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            delete m_chart->series().at(_lineNumber);
    }

    void setShowLine(unsigned int _lineNumber){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            if(!m_chart->series().at(_lineNumber)->isVisible())
                m_chart->series().at(_lineNumber)->show();
    }

    void setHideLine(unsigned int _lineNumber){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            if(m_chart->series().at(_lineNumber)->isVisible())
                m_chart->series().at(_lineNumber)->hide();
    }

    void  setColorLine(unsigned int _lineNumber, QColor _color){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->setColor(_color);
    }

    void  setStyleLine(unsigned int _lineNumber, Qt::PenStyle _style){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1)){
            QPen _pen = reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->pen();
            _pen.setStyle(_style);
            reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->setPen(_pen);
        }
    }

    void  setPenLine(unsigned int _lineNumber, QPen _pen){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->setPen(_pen);
    }

    void  setWidthFLine(unsigned int _lineNumber, qreal _thickness){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1)){
            QPen _pen = reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->pen();
            _pen.setWidthF(_thickness);
            reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->setPen(_pen);
        }
    }

    QColor getLineColor(unsigned int _lineNumber){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            return reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->color();
        return QColor();
    }

    Qt::PenStyle getLineStyle(unsigned int _lineNumber){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            return reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->pen().style();
        return Qt::PenStyle();
    }

    qreal getLineWidthF(unsigned int _lineNumber){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            return reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->pen().widthF();
        return 0.0;
    }

    QPen getLinePen(unsigned int _lineNumber){
        if(_lineNumber < static_cast<unsigned int>(m_chart->series().count()-1))
            return reinterpret_cast<QLineSeries*>(m_chart->series().at(_lineNumber))->pen();
        return QPen();
    }

    //work with legend
    void addLegend(){
        m_chart->legend()->setEnabled(true);
        m_chart->legend()->setVisible(true);

    }

    void setLegendAlign(Qt::Alignment _align){
        m_chart->legend()->setAlignment(_align);
    }
private:
    void resetMinMax(){
        m_axisMaxY=std::numeric_limits<double>::min();
        m_axisMaxX=std::numeric_limits<double>::min();
        m_axisMinX=std::numeric_limits<double>::max();
        m_axisMinY=std::numeric_limits<double>::max();
    }
    //
    QChart *m_chart = nullptr;
    //indexses
    size_t countColors = 0;
    //
    QString titleName;
    QString m_axisXName = "X",m_axisYName = "Y";
    //
    std::list<QString> m_LinesNames;
    //max point size
    unsigned int xPointSize;
    unsigned int yPointSize;
    //chart points
    QList<QPointF> m_chartPoints;
    //
    std::vector<QString> m_colorsVector;
    //Series
    QLineSeries *m_lineSeries;
    //Axis
    QValueAxis *axisX = nullptr;
    QValueAxis *axisY = nullptr;
    //min max values
    double m_axisMaxY=std::numeric_limits<double>::min(),
    m_axisMaxX=std::numeric_limits<double>::min(),
    m_axisMinX=std::numeric_limits<double>::max(),
    m_axisMinY=std::numeric_limits<double>::max(),
    m_axisStepX = 1,
    m_axisStepY = 1;
    //
    QFont fontType;
    bool statDynamFlagH = false,statDynamFlagV = false;
    qreal m_verticalInterval = 0,
    m_horizontalInterval = 0,
    tickIntervalV = 0,
    tickIntervalH = 0,
    m_gridHStep = 20,
    m_gridVStep = 20;
};

class SkyChart_base : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SkyChart_base(QWidget *parent = nullptr);
    ~SkyChart_base();
    bool createOrAdd2DChart(std::vector<float> *_x, std::vector<float> *_y, QString _LineName = "chartLine");
    QWidget *skyChart();
private:
    Base2DChart* baseChart;
    QRubberBand* m_rubberBand;
    QPoint m_pressButtonPosition;
    bool pressMouse;
    void screenshot();
    void copy();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

signals:

};

#endif // SKYCHART_BASE_H

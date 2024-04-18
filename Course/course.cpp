#include "Course.h"
//конструктор вікна
Course::Course(QWidget *parent): QMainWindow(parent)
{
	this->resize(500, 500);//змінюємо розмір вікна за замовчуванням
	this->setMouseTracking(true);//дозволяємо обробляти рух мишкою
	setWindowIcon(QIcon("icon.jpg"));//встановлюємо іконку програми
	//розміри кнопок
	moveVertexBtn->setGeometry(10, 10, 50, 50);
	addVertexBtn->setGeometry(65,10,50,50);
	addEdgeBtn->setGeometry(120,10,50,50);
	deleteBtn->setGeometry(175,10,50,50);
	findPathBtn->setGeometry(230,10,50,50);
	//підказки
	moveVertexBtn->setToolTip("Переміщення вершини(CTRL+1)");
	addVertexBtn->setToolTip("Додати вершину(CTRL+2)");
	addEdgeBtn->setToolTip("Додати ребро(CTRL+3)");
	deleteBtn->setToolTip("Видалити об'єкт(CTRL+4)");
	findPathBtn->setToolTip("Знайти найкоротший шлях(CTRL+5)");
	//гарячі клавіші
	moveVertexBtn->setShortcut(tr("CTRL+1"));
	addVertexBtn->setShortcut(tr("CTRL+2"));
	addEdgeBtn->setShortcut(tr("CTRL+3"));
	deleteBtn->setShortcut(tr("CTRL+4"));
	findPathBtn->setShortcut(tr("CTRL+5"));
	//додаємо ікноки до кнопок
	moveVertexBtn->setIcon(*moveIco);
	addVertexBtn->setIcon(*addIco);
	addEdgeBtn->setIcon(*wayIco);
	deleteBtn->setIcon(*deleteIco);
	findPathBtn->setIcon(*findPathIco);
	//поєднуємо кнопки з відповідними їм функціями
	connect(moveVertexBtn, &QPushButton::clicked, this, &Course::moveVertexMode);
	connect(addVertexBtn, &QPushButton::clicked, this, &Course::addVertexMode);
	connect(addEdgeBtn, &QPushButton::clicked, this, &Course::addEdgeMode);
	connect(deleteBtn, &QPushButton::clicked, this, &Course::deleteMode);
	connect(findPathBtn, &QPushButton::clicked, this, &Course::findPathMode);
	//відновлення інформації з файлу
	this->vertexID = graph.restoreFromFile();
	undoAction.addState(graph);
}
//деструктор вікна
Course::~Course()
{
	graph.saveToFile();//зберігання даних при закритті
}
//метод малювання вікна
void Course::paintEvent(QPaintEvent *event)
{
	event->accept();
	QPainter painter(this); //створюємо обєкт за допомогою якого будемо малювати
	//малюємо сіру горизонтальну лінію вгорі вікна щоб візуально відділити меню режимів від робочого поля
	painter.setBrush(QBrush(Qt::lightGray));
	painter.drawRect(0,0,this->geometry().width(),70);
	if((mode == findPath)&&(startVertex>=0)&&(finishVertex>=0))
		//якщо режим "Знайти найкоротший шлях" і обрали початкову та кінцеву вершини
	{
		this->graph.algo.graphChange(this->graph.ways);
		graph.algo.findShortestPath(startVertex,finishVertex);//шукаємо накоротший шлях
		graph.drawPathLenght(painter,finishVertex);//малюємо прямокутник з довжиною цього шляху
	}
	//малювання реброів між вершинами
	graph.drawEdges(painter);

	//малюємо вершини
	graph.drawVertices(painter, startVertex);

}
//обробка руху миші
void Course::mouseMoveEvent(QMouseEvent *event)
{
	if(draggedVertex >= 0)//якщо ми захопили вершину для переміщення
	{
		graph.moveVertex(startGrab + (event->pos() - startGrab),draggedVertex);//переміщюжмо вершину
		repaint();
	}
	else if(startVertex >= 0)//якщо шукаємо найкоротший шлях і вибрали початкову вершину
	{
		finishVertex = graph.hitVertex(event->pos());//скинули до значення за замовчуванням
		repaint();
	}
}
//відпускання кнопок миші
void Course::mouseReleaseEvent(QMouseEvent *event)
{
	event->accept();
	if(draggedVertex >= 0)//якщо ми захопили вершину для переміщення
	{
		//обнуляємо значення
		startGrab = QPoint();
		draggedVertex = -1;
		undoAction.addState(graph);
	}
}
//обробка натиснення клавіш миші
void Course::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) //якщо натиснули ЛКМ
	{
		switch (mode)
		{
		case moveVertex://режим "Переміщення вершини"
		{
			if(draggedVertex == -1) //якщо не захопили вершину
			{
				draggedVertex = graph.hitVertex(event->pos());//записуємо її значення
				if(draggedVertex >= 0)//якщо ми захопили вершину для переміщення
				{
					startGrab = graph.vertices[graph.hitVertex(event->pos())];//записуємо координати вершини в яку влучили
				}
			}
			break;
		}
		case addVertex://режим "Додати вершину"
		{
			if(event->pos().y()>85)//якщо не потрапили в меню
			{
				graph.addVertex(event->pos(),vertexID);
				vertexID++;
				undoAction.addState(graph);
				repaint();
			}
			break;
		}
		case addEdge://режим "Додати ребро"
		{
			startVertex = graph.addEdge(event->pos(),startVertex);
			undoAction.addState(graph);
			repaint();
			break;
		}
		case deleteObject://режим "Видалити об'єкт"
		{
			graph.deleteVertex(event->pos());
			graph.deletedEdge(event->pos());
			undoAction.addState(graph);
			repaint();
			break;
		}
		case findPath://режим "Знайти найкоротший шлях"
		{
			startVertex = graph.hitVertex(event->pos());;//записуємо її номер
			repaint();
			break;
		}
		default:
			break;
		}
	}
}
//гаряча клавіша ctrl+z(відміна останньої дії)
void Course::keyPressEvent(QKeyEvent *event)
{
	if ((event->key() == Qt::Key_Z)&&(event->modifiers()==Qt::ControlModifier))
	{
		if(undoAction.graphs.size()>1)//щоб не вийти за межі масиву
		{
			undoAction.graphs.pop_back();//змінюємо поточне значення на перед останнє у списку
			graph.numbers.resize(undoAction.graphs.last().numbers.size());
			graph.vertices.resize(undoAction.graphs.last().vertices.size());
			graph.ways.resize(undoAction.graphs.last().ways.size());
			graph.numbers = undoAction.graphs.last().numbers;
			graph.vertices = undoAction.graphs.last().vertices;
			graph.ways = undoAction.graphs.last().ways;
			repaint();
		}
	}
}
//якщо натиснули на кнопку режиму "Переміщення вершини"
void Course::moveVertexMode()
{
	mode = moveVertex;
	Icons();
}
//якщо натиснули на кнопку режиму "Додати вершину"
void Course::addVertexMode()
{
	mode = addVertex;
	Icons();
}
//якщо натиснули на кнопку режиму "Додати ребро"
void Course::addEdgeMode()
{
	mode = addEdge;
	Icons();
}
//якщо натиснули на кнопку режиму "Видалити об'єкт"
void Course::deleteMode()
{
	mode = deleteObject;
	Icons();
}
//якщо натиснули на кнопку режиму "Знайти найкоротший шлях"
void Course::findPathMode()
{
	mode = findPath;
	Icons();
}
//зміна іконок на кнопках
void Course::Icons()
{
	startVertex = -1; //"обнуляємо" значення щоб не виникло плутанини
	//змінюємо іконки кнопок
	moveVertexBtn->setIcon(*moveIco);
	addVertexBtn->setIcon(*addIco);
	addEdgeBtn->setIcon(*wayIco);
	deleteBtn->setIcon(*deleteIco);
	findPathBtn->setIcon(*findPathIco);
	switch (mode)
	{
	case moveVertex:
		moveVertexBtn->setIcon(*moveIcoSet);
		break;
	case addVertex:
		addVertexBtn->setIcon(*addIcoSet);
		break;
	case addEdge:
		addEdgeBtn->setIcon(*wayIcoSet);
		break;
	case deleteObject:
		deleteBtn->setIcon(*deleteIcoSet);
		break;
	case findPath:
		findPathBtn->setIcon(*findPathIcoSet);
		break;
	default:
		break;
	}
	repaint();
}

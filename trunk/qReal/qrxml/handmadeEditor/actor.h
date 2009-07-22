#pragma once

#include "../../qrgui/umllib/uml_nodeelement.h"

#include <QDebug>

namespace UML {

	class Actor : public NodeElement {
	public:
		Actor() {
			mRenderer.load(QString(":/TestEditor/UseCase/Actor.sdf"));
			mPortRenderer.load(QString(":/TestEditor/UseCase/ActorPorts.sdf"));
			m_contents.setWidth(100);
			m_contents.setHeight(100);
		}

		virtual ~Actor() {}

		void paint(QPainter *painter, QStyleOptionGraphicsItem const *style, QWidget *widget)
		{
			// static int paintCount = 0;
			// ++paintCount;
			// qDebug() << "Paint " << paintCount;

			mRenderer.render(painter, m_contents);
			NodeElement::paint(painter, style, widget, &mPortRenderer);
		}

		void updateData()
		{
			NodeElement::updateData();
			update();
		}

	private:
		SdfRenderer mRenderer;
		SdfRenderer mPortRenderer;
	};
}


/***

  Olive - Non-Linear Video Editor
  Copyright (C) 2019 Olive Team

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

***/

#include "timeline.h"

#include <QScrollBar>
#include <QVBoxLayout>

TimelinePanel::TimelinePanel(QWidget *parent) :
  PanelWidget(parent)
{
  // FIXME: This won't work if there's ever more than one of this panel
  setObjectName("TimelinePanel");

  QWidget* main = new QWidget(this);
  setWidget(main);

  QVBoxLayout* layout = new QVBoxLayout(main);
  layout->setSpacing(0);
  layout->setMargin(0);

  ruler_ = new TimeRuler(true, this);
  layout->addWidget(ruler_);

  view_ = new TimelineView(this);
  layout->addWidget(view_);

  connect(view_->horizontalScrollBar(), SIGNAL(valueChanged(int)), ruler_, SLOT(SetScroll(int)));
  connect(view_, SIGNAL(ScaleChanged(double)), this, SLOT(SetScale(double)));
  connect(view_, SIGNAL(TimebaseChanged(const rational&)), this, SLOT(SetTimebase(const rational&)));
  connect(ruler_, SIGNAL(TimeChanged(const int64_t&)), view_, SLOT(SetTime(const int64_t&)));
  connect(view_, SIGNAL(TimeChanged(const int64_t&)), ruler_, SLOT(SetTime(const int64_t&)));
  connect(view_, SIGNAL(TimeChanged(const int64_t&)), this, SIGNAL(TimeChanged(const int64_t&)));
  connect(ruler_, SIGNAL(TimeChanged(const int64_t&)), this, SIGNAL(TimeChanged(const int64_t&)));

  // FIXME: Magic number
  SetScale(90.0);

  Retranslate();
}

void TimelinePanel::Clear()
{
  SetTimebase(0);

  view_->Clear();
}

void TimelinePanel::SetTimebase(const rational &timebase)
{
  ruler_->SetTimebase(timebase);
  view_->SetTimebase(timebase);
}

void TimelinePanel::SetTime(const int64_t &timestamp)
{
  ruler_->SetTime(timestamp);
  view_->SetTime(timestamp);
}

TimelineView *TimelinePanel::view()
{
  return view_;
}

void TimelinePanel::ConnectTimelineNode(TimelineOutput *node)
{
  view_->ConnectTimelineNode(node);
}

void TimelinePanel::DisconnectTimelineNode()
{
  view_->DisconnectTimelineNode();
}

void TimelinePanel::ZoomIn()
{
  SetScale(scale_ * 2);
}

void TimelinePanel::ZoomOut()
{
  SetScale(scale_ * 0.5);
}

void TimelinePanel::SelectAll()
{
  view_->SelectAll();
}

void TimelinePanel::DeselectAll()
{
  view_->DeselectAll();
}

void TimelinePanel::RippleToIn()
{
  view_->RippleToIn();
}

void TimelinePanel::RippleToOut()
{
  view_->RippleToOut();
}

void TimelinePanel::EditToIn()
{
  view_->EditToIn();
}

void TimelinePanel::EditToOut()
{
  view_->EditToOut();
}

void TimelinePanel::GoToPrevCut()
{
  view_->GoToPrevCut();
}

void TimelinePanel::GoToNextCut()
{
  view_->GoToNextCut();
}

void TimelinePanel::changeEvent(QEvent *e)
{
  if (e->type() == QEvent::LanguageChange) {
    Retranslate();
  }
  QDockWidget::changeEvent(e);
}

void TimelinePanel::Retranslate()
{
  SetTitle(tr("Timeline"));
  SetSubtitle(tr("(none)"));
}

void TimelinePanel::SetScale(double scale)
{
  scale_ = scale;

  ruler_->SetScale(scale_);
  view_->SetScale(scale_);
}

/*
    ------------------------------------------------------------------

    This file is part of the Open Ephys GUI
    Copyright (C) 2016 Open Ephys

    ------------------------------------------------------------------

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

*/

#include "ButtonGroupManager.h"

#include <algorithm>


static const Colour COLOUR_DIVIDER (Colours::black.withAlpha (0.12f));
static const Colour COLOUR_BORDER  (Colour::fromRGB (189, 189, 189));
static const Colour COLOUR_PRIMARY (Colours::black.withAlpha (0.87f));


ButtonGroupManager::ButtonGroupManager()
    : m_isRadioButtonMode   (true)
    , m_buttonListener      (nullptr)
    , m_buttonsLookAndFeel  (nullptr)
{
    setColour (backgroundColourId,  Colour (0x0));
    setColour (outlineColourId,     COLOUR_BORDER);
}


void ButtonGroupManager::paint (Graphics& g)
{
    auto floatLocalBounds = getLocalBounds().toFloat();
    const float cornerSize = 3.f;

    // Fill background
    g.setColour (findColour (backgroundColourId));
    g.fillRoundedRectangle (floatLocalBounds.reduced (1, 1), cornerSize);

    // Draw border
    g.setColour (findColour (outlineColourId));
    g.drawRoundedRectangle (floatLocalBounds, cornerSize, 1.f);
}


void ButtonGroupManager::colourChanged()
{
    repaint();
}


void ButtonGroupManager::buttonClicked (Button* buttonThatWasClicked)
{
    if (m_buttonListener != nullptr)
        m_buttonListener->buttonClicked (buttonThatWasClicked);
}


int ButtonGroupManager::getNumButtons() const
{
    return m_buttons.size();
}


bool ButtonGroupManager::isRadioButtonMode() const
{
    return m_isRadioButtonMode;
}


void ButtonGroupManager::addButton (Button* newButton)
{
    newButton->addListener (this);
    newButton->setLookAndFeel (m_buttonsLookAndFeel);

    addAndMakeVisible (newButton);
    m_buttons.add (newButton);

    if (m_isRadioButtonMode)
    {
        newButton->setRadioGroupId (1);
    }

    resized();
}


void ButtonGroupManager::removeButton (int index)
{
    if (index < 0
        || index >= m_buttons.size())
    {
        return;
    }

    m_buttons.remove (index);
}


void ButtonGroupManager::setRadioButtonMode (bool isRadioButtonMode)
{
    m_isRadioButtonMode = isRadioButtonMode;

    const int numButtons = m_buttons.size();
    for (int i = 0; i < numButtons; ++i)
    {
        m_buttons[i]->setRadioGroupId ( (int)isRadioButtonMode);
    }

    repaint();
}


void ButtonGroupManager::setButtonsLookAndFeel (LookAndFeel* newButtonsLookAndFeel)
{
    m_buttonsLookAndFeel = newButtonsLookAndFeel;

    std::for_each (m_buttons.begin(), m_buttons.end(), [this] (Button* button)
                    { button->setLookAndFeel (m_buttonsLookAndFeel); });

    repaint();
}


void ButtonGroupManager::setButtonListener (Button::Listener* newListener)
{
    m_buttonListener = newListener;
}

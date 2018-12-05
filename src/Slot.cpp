/**********************************************************************
 * The Dwarf Adventure - 2D adventure RPG with destructible world     *
 *                                                                    *
 * Copyright(C) 2016-2019 Dmitry Stepanov a.k.a mrDIMAS a.k.a v1al    *
 *                                                                    *
 * This program is free software: you can redistribute it and/or      *
 * modify it under the terms of the GNU General Public License as     *
 * published by the Free Software Foundation, either version 3 of     *
 * the License, or (at your option) any later version.                *
 *                                                                    *
 * This program is distributed in the hope that it will be useful,    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * along with this program. If not, see https://www.gnu.org/licenses/ *
 *                                                                    *
 **********************************************************************/

#include "Precompiled.h"
#include "Slot.h"
#include "Canvas.h"
#include "Frame.h"
#include "Image.h"

void Slot::DrawSelf()
{
}

void Slot::OnResize()
{
	mFrame->SetSize(mSize);
	mImage->SetSize(mSize - Vec2f(6, 6));
}

Slot::Slot(const shared_ptr<Canvas> &canvas) : Widget(canvas)
{
	SetStencilWriteEnabled(false);
	mFrame = canvas->Create<Frame>();
	mImage = canvas->Create<Image>();
	mImage->SetPosition({ 3, 3 });
	mImage->SetLocalVisibility(false);
}

void Slot::LinkComponents()
{
	AddChild(mFrame);
	mFrame->AddChild(mImage);
}

void Slot::Update()
{
}

void Slot::SetItem(const weak_ptr<void> &item)
{
	mItem = item;
}

void Slot::SetImage(const shared_ptr<Texture> &img, const sf::FloatRect &r)
{
	if (img)
	{
		if (img->GetWidth() > img->GetHeight())
		{
			float w = mFrame->GetSize().x - 6;
			float k = w / img->GetWidth();
			mImage->SetSize({ w, k * img->GetHeight() });
		}
		else
		{
			float h = mFrame->GetSize().y - 6;
			float k = h / img->GetHeight();
			mImage->SetSize({ k * img->GetWidth(), h });
		}
		mImage->SetPosition({ (mFrame->GetSize().x - mImage->GetSize().x) / 2, (mFrame->GetSize().y - mImage->GetSize().y) / 2 });
	}
	mImage->SetLocalVisibility(img != nullptr);
	mImage->SetTexture(img);
	mImage->SetTextureRectangle(r);
}

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

#pragma once

#include "ItemType.h"
#include "Serializer.h"

class Dialog;
class DialogNode;
class Quest;

class Phrase
{
public:
	enum Flags
	{
		// Used for answers like "(return)" and suppresses question of next node 
		SuppressNextNodeQuestion = BIT(0),
		// Used to disable branch in dialog
		Disabled = BIT(1)
	};

	Phrase();
	Phrase(const string &text);
	Phrase* SetOnConfirm(const function<void()> &onConfirm);
	Phrase* SetNextNode(const shared_ptr<DialogNode>& node);
	shared_ptr<DialogNode> GetNextNode() const;
	void OnConfirm();
	void Serialize(Serializer& sav);
	bool IsEnd() const;
	const string& GetText() const;
	void SetFlags(int flags);
	bool AreFlagsSet(int flags) const;
private:
	function<void()> mOnConfirm;
	shared_ptr<DialogNode> mNextNode;
	string mText;	
	int mFlags;
};

class DialogNode
{
public:
	DialogNode();
	DialogNode(const string& question);
	void SetQuestion(const string& question);
	string GetQuestion() const;
	void AddAnswer(const shared_ptr<Phrase>& answer);
	const vector<shared_ptr<Phrase>> &GetAnswerList() const;
	void Serialize(Serializer & sav);
	void AppendToQuestion(const string& str);
private:
	string mQuestion;
	vector<shared_ptr<Phrase>> mAnswerList;
};

class Dialog : public enable_shared_from_this<Dialog>
{
public:
	Dialog();
	Dialog(const shared_ptr<Actor>& owner);
	~Dialog();
	void SetEntryNode(const shared_ptr<DialogNode>& node);
	shared_ptr<Actor> GetOwner() const;
	shared_ptr<DialogNode> GetRoot() const;
	virtual void Serialize(Serializer & sav);
protected:
	// Small helpers
	static shared_ptr<DialogNode> MakeNode(const string& stringId);
	static shared_ptr<Phrase> MakePhrase(const string& stringId);
	// Makes "(return)" phrase
	static shared_ptr<Phrase> MakeReturnPhrase(const shared_ptr<DialogNode>& nextNode);
	static shared_ptr<Phrase> MakeDialogEndPhrase();

	weak_ptr<Actor> mOwner;	
	shared_ptr<DialogNode> mEntryNode;
	shared_ptr<DialogNode> mCurrentNode;
};

// dialog gui
class DialogGUI
{
public:
	DialogGUI();
	~DialogGUI();
	void Clear();
	void SetDialog(const shared_ptr<Dialog> &dialog);
	void Update();
	void SetVisible(bool state);
	bool IsVisible() const;

private:
	void SetDialogNode(const shared_ptr<DialogNode> &node, bool suppressQuestion);

	// View
	shared_ptr<Canvas> mCanvas;
	shared_ptr<Window> mWindow;
	shared_ptr<Label> mTextField;
	shared_ptr<Image> mSelector;
	shared_ptr<Button> mClose;
	weak_ptr<Dialog> mDialog;
	shared_ptr<ScrollableArea> mTextArea;
	shared_ptr<ScrollableArea> mPlayerAnswersArea;	
	shared_ptr<Tooltip> mTooltip;

	// ViewModel
	struct Element
	{
		shared_ptr<Label> mLabel;
		shared_ptr<Phrase> mPhrase;

		Element();
		Element(const shared_ptr<Label> & label, const shared_ptr<Phrase> & phrase);
	};

	vector<Element> mAnswerList;
};

class GameStoryDialog : public Dialog
{
public:
	GameStoryDialog();
	GameStoryDialog(const shared_ptr<Actor>& owner);
};

class BlacksmithDialog : public Dialog
{
public:
	BlacksmithDialog();
	BlacksmithDialog(const shared_ptr<Actor>& owner);

	virtual void Serialize(Serializer & sav) override;
private:
	void SetActions();

	shared_ptr<Phrase> mBarterPhrase;
};

class ThiefTalkDialog : public Dialog
{
public:
	ThiefTalkDialog();
	ThiefTalkDialog(const shared_ptr<Actor>& owner)
		: Dialog(owner)
	{
		mEntryNode = MakeNode("dialogEnd"); // stub
	}
};

class FirstDeathDialog : public Dialog
{
public:
	FirstDeathDialog();
	FirstDeathDialog(const shared_ptr<Actor>& owner);
};

class FirstWarningDialog : public Dialog
{
public:
	FirstWarningDialog();
	FirstWarningDialog(const shared_ptr<Actor>& owner);
	virtual void Serialize(Serializer & sav) override;
private:
	void SetActions();

	shared_ptr<Phrase> mChillOption1;
	shared_ptr<Phrase> mChillOption2;	
};

class BadReputationDialog : public Dialog
{
public:
	BadReputationDialog();
	BadReputationDialog(const shared_ptr<Actor>& owner);

	virtual void Serialize(Serializer & sav) override;
private:
	void SetActions();

	shared_ptr<Phrase> mThreatPhrase;
};

// Always contains random rumor dialog + some generic quest
class LocalTalkDialog : public Dialog
{
public:
	LocalTalkDialog();
	LocalTalkDialog(const shared_ptr<Actor>& owner);

	virtual void Serialize(Serializer & sav) override;
private:
	string GetRandomRumorKey() const;
	void SetActions();
	shared_ptr<Quest> mQuest;
	shared_ptr<DialogNode> mRumorNode;
	shared_ptr<Phrase> mRumorPhrase;
	ItemType mItemToFind;
	shared_ptr<Phrase> mQuestBeginPhrase;
	shared_ptr<Phrase> mQuestAcceptPhrase;
};

class MerchantTalkDialog : public Dialog
{
public:
	MerchantTalkDialog();
	MerchantTalkDialog(const shared_ptr<Actor>& owner);

	virtual void Serialize(Serializer & sav) override;
private:
	void SetActions();
	shared_ptr<Phrase> mBarter;
};

class ApologizeDialog : public Dialog
{
public:
	ApologizeDialog();
	ApologizeDialog(const shared_ptr<Actor>& owner);

	virtual void Serialize(Serializer & sav) override;
private:
	void SetActions();

	shared_ptr<Phrase> mChillOut;
};

class IllegalZoneDialog : public Dialog
{
public:
	IllegalZoneDialog();
	IllegalZoneDialog(const shared_ptr<Actor>& owner);

	virtual void Serialize(Serializer & sav) override;
private:
	void SetActions();

	shared_ptr<Phrase> mICameForYou;
};

class KickOutOfRoomDialog : public Dialog
{
public:
	KickOutOfRoomDialog();
	KickOutOfRoomDialog(const shared_ptr<Actor>& owner);
};
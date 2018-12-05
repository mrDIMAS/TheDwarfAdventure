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
#include "Dialog.h"
#include "Input.h"
#include "Window.h"
#include "Image.h"
#include "ScrollableArea.h"
#include "Button.h"
#include "Tooltip.h"
#include "Actor.h"
#include "NPC.h"
#include "BarterWindow.h"
#include "Level.h"
#include "Item.h"
#include "QuestBase.h"
#include "Player.h"

DialogGUI::DialogGUI()
{
	mCanvas = game->CreateCanvas();

	Vec2f windowSize = { 600, 300 };

	mWindow = mCanvas->Create<Window>();
	mWindow->SetSize(windowSize);
	mWindow->SetCaption(game->GetStr("conversation"));
	mWindow->SetAutoLayoutingEnabled(false);
	mWindow->AddHelpButton();
	mWindow->GetHelpButton()->OnHover += [this]
	{
		mTooltip->Show(game->GetStr("DialogUI_Help"));
	};

	mWindow->SetInCenter();

	Vec2f offset = { 10, 40 };

	mTextArea = mCanvas->Create<ScrollableArea>();
	mTextArea->SetPosition(offset);
	mTextArea->SetSize({ windowSize.x / 1.5f - 2 * offset.x, windowSize.y - offset.y - 10 });
	mWindow->AddChild(mTextArea);

	float w = windowSize.x - mTextArea->GetSize().x - 3 * offset.x;

	mClose = mCanvas->Create<Button>();
	mClose->SetSize({ w, 30 });
	mClose->SetText(game->GetStr("close"));
	mClose->OnClick += [this] { SetVisible(false); };
	mWindow->AddChild(mClose);

	mPlayerAnswersArea = mCanvas->Create<ScrollableArea>();
	mPlayerAnswersArea->SetPosition({ mTextArea->GetPosition().x + mTextArea->GetSize().x + 10, offset.y });
	mPlayerAnswersArea->SetSize({ w, windowSize.y - offset.y - 10 - mClose->GetSize().y });
	mWindow->AddChild(mPlayerAnswersArea);

	mClose->SetPosition({ mPlayerAnswersArea->GetPosition().x,
		mPlayerAnswersArea->GetPosition().y + mPlayerAnswersArea->GetSize().y + 3 });

	mSelector = mCanvas->Create<Image>();
	mSelector->SetColor({ 100, 120, 100, 120 });

	mTextField = mCanvas->Create<Label>();
	mTextField->SetAutoHeight(true);
	mTextArea->AddChild(mTextField);

	mTooltip = mCanvas->Create<Tooltip>();
	mTooltip->SetSize({ 340, 420 });

	SetVisible(false);

	Log() << "Dialog system successfully initialized!" << LineEnd;
}

DialogGUI::~DialogGUI()
{
	DBG_ECHO();
}

inline void DialogGUI::Clear()
{
	for (const Element & element : mAnswerList)
	{
		element.mLabel->RemoveFromCanvas();
	}
	mAnswerList.clear();
	mPlayerAnswersArea->RemoveAllChildren();
}

void DialogGUI::SetDialog(const shared_ptr<Dialog>& dialog)
{
	mDialog = dialog;
	SetVisible(true);
	mTextField->SetText("");
	Clear();
	SetDialogNode(dialog->GetRoot(), false);
	mTextArea->ScrollTop();
}

void DialogGUI::Update()
{
	auto input = game->GetInput();
	for (const Element & element : mAnswerList)
	{
		const shared_ptr<Label> label = element.mLabel;
		const shared_ptr<Phrase> phrase = element.mPhrase;

		if (label->IsHovered())
		{
			label->AddChild(mSelector);
			mSelector->SetSize(label->GetSize());
			if (input->IsMouseButtonUp(sf::Mouse::Left))
			{
				if (phrase->IsEnd())
				{
					SetVisible(false);
				}
				else
				{
					mTextField->AppendText("\n" + ColorToStr({ 0, 200, 0 }) + game->GetStr("you") + phrase->GetText() + ColorToStr({ 255, 255, 255 }));				
					mTextField->Update();
					mTextArea->ScrollBottom();

					SetDialogNode(phrase->GetNextNode(), phrase->AreFlagsSet(Phrase::SuppressNextNodeQuestion));
				}

				phrase->OnConfirm();

				break;
			}
		}
	}
}

void DialogGUI::SetVisible(bool state)
{
	mCanvas->SetVisible(state);
	if (!state)
	{
		mTextArea->ResetScrollBar();
	}
}

bool DialogGUI::IsVisible() const
{
	return mCanvas->IsVisible();
}

void DialogGUI::SetDialogNode(const shared_ptr<DialogNode>& node, bool suppressQuestion)
{
	Clear();
	if (!suppressQuestion)
	{
		mTextField->AppendText('\n' + node->GetQuestion());
	}
	int n = 1;
	for (const auto& phrase : node->GetAnswerList())
	{
		if (!phrase->AreFlagsSet(Phrase::Disabled))
		{
			auto lbl = mCanvas->Create<Label>();
			lbl->SetAutoHeight(true);
			lbl->SetText(to_string(n++) + ". " + phrase->GetText());
			mPlayerAnswersArea->AddChild(lbl);
			mAnswerList.push_back(Element(lbl, phrase));
		}
	}
	SetVisible(true);
}

DialogGUI::Element::Element()
{

}

DialogGUI::Element::Element(const shared_ptr<Label>& label, const shared_ptr<Phrase>& phrase)
	: mLabel(label)
	, mPhrase(phrase)
{

}

Dialog::Dialog()
{
}

Dialog::Dialog(const shared_ptr<Actor>& owner)
	: mOwner(owner)
{

}

Dialog::~Dialog()
{
	DBG_ECHO();
}

void Dialog::SetEntryNode(const shared_ptr<DialogNode>& node)
{
	mEntryNode = node;
}

shared_ptr<Actor> Dialog::GetOwner() const
{
	return mOwner.lock();
}

shared_ptr<DialogNode> Dialog::GetRoot() const
{
	return mEntryNode;
}

void Dialog::Serialize(Serializer & sav)
{
	sav & mOwner;
	sav & mEntryNode;
	sav & mCurrentNode;
}

// Small helpers

shared_ptr<DialogNode> Dialog::MakeNode(const string & stringId)
{
	return make_shared<DialogNode>(game->GetStr(stringId));
}

shared_ptr<Phrase> Dialog::MakePhrase(const string & stringId)
{
	return make_shared<Phrase>(game->GetStr(stringId));
}

shared_ptr<Phrase> Dialog::MakeReturnPhrase(const shared_ptr<DialogNode>& nextNode)
{
	auto phrase = MakePhrase("return");
	phrase->SetFlags(Phrase::SuppressNextNodeQuestion);
	phrase->SetNextNode(nextNode);
	return phrase;
}

shared_ptr<Phrase> Dialog::MakeDialogEndPhrase()
{
	return MakePhrase("dialogEnd");
}

DialogNode::DialogNode()
{
}

DialogNode::DialogNode(const string & question) 
	: mQuestion(question)
{

}

void DialogNode::SetQuestion(const string & question)
{
	mQuestion = question;
}

string DialogNode::GetQuestion() const
{
	return mQuestion;
}

void DialogNode::AddAnswer(const shared_ptr<Phrase>& answer)
{
	mAnswerList.push_back(answer);
}

const vector<shared_ptr<Phrase>>& DialogNode::GetAnswerList() const
{
	return mAnswerList;
}

void DialogNode::Serialize(Serializer & sav)
{
	sav & mQuestion;
	sav & mAnswerList;
}

void DialogNode::AppendToQuestion(const string & str)
{
	mQuestion += str;
}

Phrase::Phrase()
{
}

Phrase::Phrase(const string & text)
	: mText(text)
{
}

Phrase * Phrase::SetOnConfirm(const function<void()>& onConfirm)
{
	mOnConfirm = onConfirm;
	return this;
}

Phrase * Phrase::SetNextNode(const shared_ptr<DialogNode>& node)
{
	mNextNode = node;
	return this;
}

shared_ptr<DialogNode> Phrase::GetNextNode() const
{
	return mNextNode;
}

void Phrase::OnConfirm()
{
	if (mOnConfirm)
	{
		mOnConfirm();
	}
}

void Phrase::Serialize(Serializer & sav)
{
	sav & mNextNode;
	sav & mText;
	sav & mFlags;
}

bool Phrase::IsEnd() const
{
	return mNextNode == nullptr;
}

const string & Phrase::GetText() const
{
	return mText;
}

void Phrase::SetFlags(int flags)
{
	mFlags |= flags;
}

bool Phrase::AreFlagsSet(int flags) const
{
	return (mFlags & flags) == flags;
}

GameStoryDialog::GameStoryDialog()
{

}

GameStoryDialog::GameStoryDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	// Create nodes
	mEntryNode = MakeNode("Dialog_GameBeginning_Q_Entry");	
	auto okayNode = MakeNode("Dialog_GameBeginning_Q_OkiDoki");
	auto lazyNode = MakeNode("Dialog_GameBeginning_Q_LazyDev");

	// Entry node
	{
		mEntryNode->AddAnswer(MakePhrase("Dialog_GameBeginning_A_Ok"));
		
		{
			auto okay = MakePhrase("Dialog_GameBeginning_A_Allrigth");
			okay->SetNextNode(okayNode);
			mEntryNode->AddAnswer(okay);
		}

		{
			auto lazyDeveloper = MakePhrase("Dialog_GameBeginning_A_WhyNotSee");
			lazyDeveloper->SetNextNode(lazyNode);
			mEntryNode->AddAnswer(lazyDeveloper);
		}
	}
	
	okayNode->AddAnswer(MakeDialogEndPhrase());
	lazyNode->AddAnswer(MakeDialogEndPhrase());	
}

ApologizeDialog::ApologizeDialog()
{

}

ApologizeDialog::ApologizeDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	ASSERT(owner, "Owner cannot be nullptr here!");

	mEntryNode = make_shared<DialogNode>(game->GetStr("qApologize_q0"));

	mChillOut = make_shared<Phrase>(game->GetStr("qApologize_a0"));
	mEntryNode->AddAnswer(mChillOut);

	const shared_ptr<Phrase> no = make_shared<Phrase>(game->GetStr("qApologize_a1"));
	mEntryNode->AddAnswer(no);

	SetActions();
}

void ApologizeDialog::Serialize(Serializer & sav)
{
	Dialog::Serialize(sav);

	sav & mChillOut;

	if (sav.IsLoading())
	{
		SetActions();
	}
}

void ApologizeDialog::SetActions()
{
	mChillOut->SetOnConfirm([this]
	{
		if (const shared_ptr<NPC> npc = dynamic_pointer_cast<NPC>(mOwner.lock()))
		{
			npc->ChillOut();
		}
	});
}

BadReputationDialog::BadReputationDialog()
{

}

BadReputationDialog::BadReputationDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	ASSERT(owner, "Owner cannot be nullptr here!");

	mEntryNode = MakeNode("qBadReputation_q0");
	const shared_ptr<DialogNode> whyNode = MakeNode("qBadReputation_why_q0");
	const shared_ptr<DialogNode> bribeNode = MakeNode("qBadReputation_bribe_q0");
	const shared_ptr<DialogNode> threatNode = MakeNode("qBadReputation_threat_q0");
	const shared_ptr<DialogNode> reformNode = MakeNode("qBadReputation_q3");
	
	// Entry node answers
	{
		auto why = MakePhrase("qBadReputation_a0");
		why->SetNextNode(whyNode);

		auto bribe = MakePhrase("qBadReputation_a1");
		bribe->SetNextNode(bribeNode);

		mThreatPhrase = MakePhrase("qBadReputation_a2");
		mThreatPhrase->SetNextNode(threatNode);

		auto reform = MakePhrase("qBadReputation_a3");
		reform->SetNextNode(reformNode);

		mEntryNode->AddAnswer(why);
		mEntryNode->AddAnswer(bribe);
		mEntryNode->AddAnswer(mThreatPhrase);
		mEntryNode->AddAnswer(reform);
	}

	whyNode->AddAnswer(MakeDialogEndPhrase());
	bribeNode->AddAnswer(MakeDialogEndPhrase());
	threatNode->AddAnswer(MakeDialogEndPhrase());	
	reformNode->AddAnswer(MakeDialogEndPhrase());	

	SetActions();
}

void BadReputationDialog::Serialize(Serializer & sav)
{
	Dialog::Serialize(sav);

	sav & mThreatPhrase;

	if (sav.IsLoading())
	{
		SetActions();
	}
}

void BadReputationDialog::SetActions()
{
	mThreatPhrase->SetOnConfirm([this]
	{
		game->GetLevel()->GetPlayer()->ChangeReputation(-3);
	});
}

BlacksmithDialog::BlacksmithDialog()
{

}

BlacksmithDialog::BlacksmithDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	ASSERT(owner, "Owner cannot be nullptr here!");

	mEntryNode = MakeNode("qBlacksmith_begin_q");
	auto whoAreYouNode = MakeNode("qBlacksmith_whoAreYou_q");
	auto howLongLiveNode = MakeNode("qBlacksmith_howLongLive_q");
	auto benchNode = MakeNode("qBlacksmith_bench_q");
	auto endNode = MakeNode("qBlacksmith_end_q");

	{
		auto whoAreYou = MakePhrase("qBlacksmith_begin_a0");
		whoAreYou->SetNextNode(whoAreYouNode);

		mBarterPhrase = MakePhrase("qBlacksmith_begin_a1");

		auto end = MakePhrase("qBlacksmith_begin_a2");
		end->SetNextNode(endNode);

		auto bench = MakePhrase("qBlacksmith_begin_a3");
		bench->SetNextNode(benchNode);

		mEntryNode->AddAnswer(whoAreYou);
		mEntryNode->AddAnswer(mBarterPhrase);
		mEntryNode->AddAnswer(bench);
		mEntryNode->AddAnswer(end);
	}
		
	whoAreYouNode->AddAnswer(MakeReturnPhrase(mEntryNode));

	howLongLiveNode->AddAnswer(MakeReturnPhrase(mEntryNode));

	benchNode->AddAnswer(MakeReturnPhrase(mEntryNode));

	endNode->AddAnswer(MakeDialogEndPhrase());	

	SetActions();
}

void BlacksmithDialog::Serialize(Serializer & sav)
{
	Dialog::Serialize(sav);

	sav & mBarterPhrase;

	if (sav.IsLoading())
	{
		SetActions();
	}
}

void BlacksmithDialog::SetActions()
{
	mBarterPhrase->SetOnConfirm([this]
	{
		game->GetBarterWindow()->Open(mOwner.lock(), game->GetLevel()->GetPlayer());
	});
}

FirstWarningDialog::FirstWarningDialog()
{

}

FirstWarningDialog::FirstWarningDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	ASSERT(owner, "Owner cannot be nullptr here!");

	mEntryNode = MakeNode("Dialog_FirstWarning_Q_Entry");

	mChillOption1 = MakePhrase("Dialog_FirstWarning_A_Sorry");
	mChillOption2 = MakePhrase("Dialog_FirstWarning_A_Ok");

	mEntryNode->AddAnswer(mChillOption1);
	mEntryNode->AddAnswer(mChillOption2);
	mEntryNode->AddAnswer(MakePhrase("Dialog_FirstWarning_A_YouWillDie"));

	SetActions();
}

void FirstWarningDialog::Serialize(Serializer & sav)
{
	Dialog::Serialize(sav);

	sav & mChillOption1;
	sav & mChillOption2;

	if (sav.IsLoading())
	{
		SetActions();
	}
}

void FirstWarningDialog::SetActions()
{
	auto chill = [this]
	{
		if (auto owner = dynamic_pointer_cast<NPC>(mOwner.lock()))
		{
			owner->ChillOut();
		}
	};

	mChillOption1->SetOnConfirm(chill);
	mChillOption2->SetOnConfirm(chill);
}

FirstDeathDialog::FirstDeathDialog()
{

}

FirstDeathDialog::FirstDeathDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{

	mEntryNode = MakeNode("qRareRedCrystal_begin_q");
	mEntryNode->AddAnswer(MakeDialogEndPhrase());
}

MerchantTalkDialog::MerchantTalkDialog()
{

}

MerchantTalkDialog::MerchantTalkDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	ASSERT(owner, "Owner cannot be nullptr here!");

	mEntryNode = MakeNode("Dialog_Merchant_Q_Entry");

	auto whatsWrong = MakeNode("Dialog_Merchant_Q_Monsters");
	whatsWrong->AddAnswer(MakeReturnPhrase(mEntryNode));

	mBarter = MakePhrase("Dialog_Merchant_A_Barter");

	mEntryNode->AddAnswer(mBarter);
	mEntryNode->AddAnswer(MakePhrase("Dialog_Merchant_A_WhatsWrong"));
	mEntryNode->AddAnswer(MakeDialogEndPhrase());

	SetActions();
}

void MerchantTalkDialog::Serialize(Serializer & sav)
{
	Dialog::Serialize(sav);

	sav & mBarter;

	if (sav.IsLoading())
	{
		SetActions();
	}
}

void MerchantTalkDialog::SetActions()
{
	mBarter->SetOnConfirm([this]
	{
		game->GetBarterWindow()->Open(mOwner.lock(), game->GetLevel()->GetPlayer());
	});
}

ThiefTalkDialog::ThiefTalkDialog()
{

}

LocalTalkDialog::LocalTalkDialog()
{

}

LocalTalkDialog::LocalTalkDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	ASSERT(owner, "Owner cannot be nullptr here!");

	mEntryNode = MakeNode("Dialog_Local_Q_Entry");
	mRumorNode = MakeNode(GetRandomRumorKey());

	mRumorPhrase = MakePhrase("Dialog_Local_A_Rumors");
	mRumorPhrase->SetNextNode(mRumorNode);
	mEntryNode->AddAnswer(mRumorPhrase);

	mRumorNode->AddAnswer(MakeDialogEndPhrase());

	// Add random quest
	{
		int dice = Math::Rnd(0, 1);

		if (dice == 0)
		{
			// Create quest
			{
				mItemToFind = ItemType::Weapon_Shotgun;
				auto item = game->GetItemDatabase()->Make(mItemToFind, owner->GetLocation());
				owner->GetLocation()->AddItem(item);
				item->SetPosition(owner->GetPosition()); // test
				mQuest = make_shared<FindItemQuest>(owner, item);
			}

			// Create nodes
			auto questBeginNode = MakeNode("Dialog_FindItemQuest_Q_Begin");
			auto acceptNode = MakeNode("Dialog_FindItemQuest_Q_Accept");
			auto rewardNode = MakeNode("Dialog_FindItemQuest_Q_Reward");
			rewardNode->AppendToQuestion(mQuest->GetRewardDescription());
			auto refuseNode = MakeNode("Dialog_FindItemQuest_Q_Refuse");

			// Add phrases and links
			mQuestBeginPhrase = MakePhrase("Dialog_AnyQuest_A_Entry");
			mQuestBeginPhrase->SetNextNode(questBeginNode);
			mEntryNode->AddAnswer(mQuestBeginPhrase);

			mQuestAcceptPhrase = MakePhrase("Dialog_AnyQuest_A_Accept");
			mQuestAcceptPhrase->SetNextNode(acceptNode);

			auto reward = MakePhrase("Dialog_AnyQuest_A_Reward");
			reward->SetNextNode(rewardNode);

			auto refuse = MakePhrase("Dialog_AnyQuest_A_Refuse");
			refuse->SetNextNode(refuseNode);

			questBeginNode->AddAnswer(mQuestAcceptPhrase);
			questBeginNode->AddAnswer(reward);
			questBeginNode->AddAnswer(refuse);

			refuseNode->AddAnswer(MakeDialogEndPhrase());
			acceptNode->AddAnswer(MakeDialogEndPhrase());
			rewardNode->AddAnswer(MakeReturnPhrase(questBeginNode));
		}
		else if (dice == 1)
		{
			// Create quest					
			mQuest = make_shared<KillQuest>(mOwner.lock(), vector<weak_ptr<Actor>>{ }, vector<sf::FloatRect>{ });
			
			// Create nodes
			auto questBeginNode = MakeNode("Dialog_KillQuest_Q_Begin");
			auto acceptNode = MakeNode("Dialog_KillQuest_Q_Accept");
			auto rewardNode = MakeNode("Dialog_KillQuest_Q_Reward");
			rewardNode->AppendToQuestion(mQuest->GetRewardDescription());
			auto refuseNode = MakeNode("Dialog_KillQuest_Q_Refuse");

			// Add phrases and links
			mQuestBeginPhrase = MakePhrase("Dialog_AnyQuest_A_Entry");
			mQuestBeginPhrase->SetNextNode(questBeginNode);
			mEntryNode->AddAnswer(mQuestBeginPhrase);

			mQuestAcceptPhrase = MakePhrase("Dialog_AnyQuest_A_Accept");
			mQuestAcceptPhrase->SetNextNode(acceptNode);

			auto reward = MakePhrase("Dialog_AnyQuest_A_Reward");
			reward->SetNextNode(rewardNode);

			auto refuse = MakePhrase("Dialog_AnyQuest_A_Refuse");
			refuse->SetNextNode(refuseNode);

			questBeginNode->AddAnswer(mQuestAcceptPhrase);
			questBeginNode->AddAnswer(reward);
			questBeginNode->AddAnswer(refuse);

			refuseNode->AddAnswer(MakeDialogEndPhrase());
			acceptNode->AddAnswer(MakeDialogEndPhrase());
			rewardNode->AddAnswer(MakeReturnPhrase(questBeginNode));
		}
	}

	SetActions();
}

void LocalTalkDialog::Serialize(Serializer & sav)
{
	Dialog::Serialize(sav);

	sav & mQuestBeginPhrase;
	sav & mQuest;
	sav & mRumorNode;
	sav & mItemToFind;
	sav & mQuestAcceptPhrase;
	sav & mRumorPhrase;

	if (sav.IsLoading())
	{
		SetActions();
	}
}

string LocalTalkDialog::GetRandomRumorKey() const
{
	constexpr const char* keys[] = {
		"Dialog_Local_Q_Rumor_0",
		"Dialog_Local_Q_Rumor_1",
		"Dialog_Local_Q_Rumor_2",
		"Dialog_Local_Q_Rumor_3",
	};

	return keys[Math::Rnd(0, LENGTH_OF(keys) - 1)];
}

void LocalTalkDialog::SetActions()
{
	if (mQuestBeginPhrase && mQuestAcceptPhrase)
	{
		mQuestAcceptPhrase->SetOnConfirm([this]
		{
			mQuestBeginPhrase->SetFlags(Phrase::Disabled);

			game->GetLevel()->AddQuest(mQuest);
		});
	}

	mRumorPhrase->SetOnConfirm([this]
	{
		mRumorNode->SetQuestion(game->GetStr(GetRandomRumorKey()));
	});
}

KickOutOfRoomDialog::KickOutOfRoomDialog()
{

}

KickOutOfRoomDialog::KickOutOfRoomDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	ASSERT(owner, "Owner cannot be nullptr here!");

	mEntryNode = MakeNode("Dialog_KickOutFromRoom_Q_Entry");
	mEntryNode->AddAnswer(MakePhrase("Dialog_KickOutFromRoom_A_JustCurious"));
	mEntryNode->AddAnswer(MakePhrase("Dialog_KickOutFromRoom_A_StillGetYou"));
}

IllegalZoneDialog::IllegalZoneDialog()
{

}

IllegalZoneDialog::IllegalZoneDialog(const shared_ptr<Actor>& owner)
	: Dialog(owner)
{
	ASSERT(owner, "Owner cannot be nullptr here!");

	mEntryNode = MakeNode("Dialog_IllegalZone_Q_Entry");
	mEntryNode->AddAnswer(MakePhrase("Dialog_IllegalZone_A_Sorry"));
	mEntryNode->AddAnswer(MakePhrase("Dialog_IllegalZone_A_FineLeave"));
	mEntryNode->AddAnswer(MakePhrase("Dialog_IllegalZone_A_DontCare"));

	mICameForYou = MakePhrase("Dialog_IllegalZone_A_ICameForYou");
	mEntryNode->AddAnswer(mICameForYou);

	SetActions();
}

void IllegalZoneDialog::Serialize(Serializer & sav)
{
	Dialog::Serialize(sav);

	sav & mICameForYou;

	if (sav.IsLoading())
	{
		SetActions();
	}
}

void IllegalZoneDialog::SetActions()
{
	mICameForYou->SetOnConfirm([this]
	{
		if (const auto owner = dynamic_pointer_cast<NPC>(mOwner.lock()))
		{
			owner->MakeHostile();
		}
	});
}

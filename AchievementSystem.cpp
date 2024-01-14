#include "stdafx.h"
#include "AchievementSystem.h"

/// AUTOGENERATED METHODS ///

int AchievementSystem::AddRef() {
	return Simulator::cStrategy::AddRef();
}
int AchievementSystem::Release() {
	return Simulator::cStrategy::Release();
}

const char* AchievementSystem::GetName() const {
	return "RattlerSpore::AchievementSystem";
}

bool AchievementSystem::Write(Simulator::ISerializerStream* stream)
{
	return Simulator::ClassSerializer(this, ATTRIBUTES).Write(stream);
}
bool AchievementSystem::Read(Simulator::ISerializerStream* stream)
{
	return Simulator::ClassSerializer(this, ATTRIBUTES).Read(stream);
}

bool AchievementSystem::WriteToXML(Simulator::XmlSerializer* writexml)
{
	return false;
}

/// END OF AUTOGENERATED METHODS ///
////////////////////////////////////

Simulator::Attribute AchievementSystem::ATTRIBUTES[] = {
	// Add more attributes here
	// This one must always be at the end

	SimAttribute(AchievementSystem,CraftCount,1),
	SimAttribute(AchievementSystem,Discoveries,1),
	SimAttribute(AchievementSystem,MineCount,1),
	Simulator::Attribute()
};



void AchievementSystem::Initialize() {
	sInstance = this;
	mpUIlayout = nullptr;

	counter = 0;
	SecretPhrase = false;

}

void AchievementSystem::Dispose() {
	
}

void AchievementSystem::Update(int deltaTime, int deltaGameTime) {
	if (Simulator::IsSpaceGame()) //If we're in the space stage... (adventures do not count)
	{


		if (mpUIlayout)
		{
			counter++;
			if (counter > 220) {
				Close();
			}

		}
	}
	else
	{
		if (mpUIlayout)
		{
			Close();
		}
	}

}

bool AchievementSystem::Achieve(string16 name, string16 desc, ImagePtr grug)
{
	if (!mpUIlayout)
	{
		mpUIlayout = new UTFWin::UILayout();
	}
	else {
	
		Close();
	}

	if (mpUIlayout->LoadByID(id("rat_achievement")))
	{	
		counter = 0;
		mpUIlayout->SetVisible(true);
		mpUIlayout->SetParentWindow(WindowManager.GetMainWindow());
		auto window = mpUIlayout->FindWindowByID(0xFFFFFFFF, false);
		window->SetSize(1601.0F, 802.0F);
		WindowManager.GetMainWindow()->SendToBack(mpUIlayout->GetContainerWindow());
		
		Math::Rectangle rec = window->GetParent()->GetArea();
		window->SetArea(Math::Rectangle(rec.right / 2 - (1601 / 2), (rec.bottom / 2 - (902 / 2)), rec.right / 2 + (1601 / 2), (rec.bottom / 2 + (902 / 2))));
		layout.SetParentWindow(window);
		mpUIlayout->FindWindowByID(id("leTitle"), true)->SetCaption(name.c_str());
		mpUIlayout->FindWindowByID(id("leDesc"), true)->SetCaption(desc.c_str());

		UTFWin::ImageDrawable* drawable = new UTFWin::ImageDrawable();
		drawable->SetImage(grug.get());
		mpUIlayout->FindWindowByID(id("leIcon"), true)->SetDrawable(drawable);

		//mpUIlayout->FindWindowByID(id("skibidiRizz"), true).
		
			
		return true;
	}
	return false;
}


bool AchievementSystem::Achieve(string ied)
{
	PropertyListPtr sillyPropList;
	ResourceKey imgKey;
	//Gets the achievment spacetool
	auto inventory = SimulatorSpaceGame.GetPlayerInventory();
	if (inventory->GetTool({ id(ied.c_str()), 0, 0 }) != nullptr) {

		return false;
	}

	if (PropManager.GetPropertyList(id(ied.c_str()), 0x30608f0b, sillyPropList))
	{
		LocalizedString toolName;
		LocalizedString toolDesc;


		if (App::Property::GetText(sillyPropList.get(), 0x3068D95D, toolName) && (App::Property::GetText(sillyPropList.get(), 0x04CAD19B, toolDesc)))
		{
			string16 achName;
			string16 achDesc;
			achName.assign_convert(toolName.GetText());
			achDesc.assign_convert(toolDesc.GetText());
			//App::ConsolePrintF("sex");
			if (App::Property::GetKey(sillyPropList.get(), 0x3068D95C, imgKey))
			{
				ImagePtr img;
				if (UTFWin::Image::GetImage(imgKey, img))
				{
					//	App::ConsolePrintF("autism");
					Achieve(achName, achDesc, img);


					cSpaceToolDataPtr tool;
					ToolManager.LoadTool({ id(ied.c_str()), 0, 0 }, tool);
					inventory->AddItem(tool.get());
					return true;
				}
			}
		}



	}
	return false;
}

bool AchievementSystem::Close()
{
	//App::ConsolePrintF("mario");
	if (mpUIlayout)
	{


		mpUIlayout->SetVisible(false);
		//App::ConsolePrintF("le test");
		WindowManager.GetMainWindow()->RemoveWindow(mpUIlayout->FindWindowByID(0xFFFFFFFF, false));
		auto Delete(mpUIlayout);
		mpUIlayout = nullptr;
		return true;
	}
	//App::ConsolePrintF("brug");
	return false;
}

bool AchievementSystem::Discover(string matname)
{
	if (std::find(Discoveries.begin(), Discoveries.end(), matname) != Discoveries.end()) {
		return true;
	}
	else {

		Discoveries.emplace_back(matname);
		/* v does not contain x */
	}

	if (Discoveries.size() == 6) {
		Achieve("ach_core_geo1");
	}
	if (Discoveries.size() == 11) {
		Achieve("ach_core_geo2");
	}
	if (Discoveries.size() == 21) {
		Achieve("ach_core_geo3");
	}
	return false;
}

AchievementSystem* AchievementSystem::Get()
{
	return sInstance;
}

AchievementSystem* AchievementSystem::sInstance;

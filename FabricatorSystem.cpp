#include "stdafx.h"
#include "FabricatorSystem.h"
#include "xFabClose.h"
#include "xFabOpen.h"
#include "RecipeIcon.h"
#include "Craft.h"

/// AUTOGENERATED METHODS ///

int FabricatorSystem::AddRef() {
	return Simulator::cStrategy::AddRef();
}
int FabricatorSystem::Release() {
	return Simulator::cStrategy::Release();
}

const char* FabricatorSystem::GetName() const {
	return "RattlerSpore::FabricatorSystem";
}

bool FabricatorSystem::Write(Simulator::ISerializerStream* stream)
{
	return Simulator::ClassSerializer(this, ATTRIBUTES).Write(stream);
}
bool FabricatorSystem::Read(Simulator::ISerializerStream* stream)
{
	return Simulator::ClassSerializer(this, ATTRIBUTES).Read(stream);
}


bool FabricatorSystem::WriteToXML(Simulator::XmlSerializer* writexml)
{
	return false;
}

/// END OF AUTOGENERATED METHODS ///
////////////////////////////////////

Simulator::Attribute FabricatorSystem::ATTRIBUTES[] = {
	// Add more attributes here
	// This one must always be at the end
	Simulator::Attribute()
};

void FabricatorSystem::Initialize() {

	mpUIlayout = nullptr;
	sInstance = this;
}

void FabricatorSystem::Dispose() {
	
}

void FabricatorSystem::Update(int deltaTime, int deltaGameTime) {
	if (Simulator::IsSpaceGame()) //If we're in the space stage... (adventures do not count)
	{
		

		if (mWindowOffset != 0 && mpUIlayout)
		{
			auto window = mpUIlayout->FindWindowByID(0xFFFFFFFF, false);
			float offset2 = (((mWindowOffset * -1) + 1) / 8); //max((((windowOffset * -1) + 1) / 8), 1.0)
			mWindowOffset = mWindowOffset + offset2;

			UTFWin::IWindow* parentWindow = window->GetParent();
			Math::Rectangle rec = parentWindow->GetArea();
			window->SetArea(Math::Rectangle(rec.right / 2 - (1601 / 2), (rec.bottom / 2 - (902 / 2)) + mWindowOffset, rec.right / 2 + (1601 / 2), (rec.bottom / 2 + (902 / 2)) + mWindowOffset));
		}
	}
	else
	{
		if (mpUIlayout)
		{
			CloseFab(false);
		}
	}
}

FabricatorSystem* FabricatorSystem::Get()
{
	return sInstance;
}

bool FabricatorSystem::OpenFab(bool sex) {
	if (!mpUIlayout)
	{
		mpUIlayout = new UTFWin::UILayout();
	}

	if (mpUIlayout->LoadByID(id("FabMenu")))
	{
		//mpUIlayout->LoadByID(id("FabMenu"));
		mpUIlayout->SetVisible(true);
		mpUIlayout->SetParentWindow(WindowManager.GetMainWindow());
		auto window = mpUIlayout->FindWindowByID(0xFFFFFFFF, false);
		window->SetSize(1601.0F, 802.0F);
		WindowManager.GetMainWindow()->SendToBack(mpUIlayout->GetContainerWindow());

		Math::Rectangle rec = window->GetParent()->GetArea();

		if (sex)
		{
			mWindowOffset = (rec.bottom / 2 + (902 / 2));
		}
		else
		{
			mWindowOffset = 0;
		}

		window->SetArea(Math::Rectangle(rec.right / 2 - (1601 / 2), (rec.bottom / 2 - (902 / 2)) + mWindowOffset, rec.right / 2 + (1601 / 2), (rec.bottom / 2 + (902 / 2)) + mWindowOffset));


		auto closeButton = mpUIlayout->FindWindowByID(id("exitRF"));
		closeButton->AddWinProc(new xFabClose());

		auto craftButton = mpUIlayout->FindWindowByID(id("craft"));
		craftButton->AddWinProc(new Craft());


		auto menuWindow = mpUIlayout->FindWindowByID(id("FabMenu"));
		layout.SetParentWindow(window);

		ReadRecipes();
		return true;
	}
	return false;
}




bool FabricatorSystem::CloseFab(bool sex) {
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

void FabricatorSystem::RenderRecipies(int cat)
{
	auto inventoryWindow = mpUIlayout->FindWindowByID(id("Inventory"), true);
	int i = 0;

	vector<Recipe> validRecipes;
	vector<Recipe> invalidRecipes;

	for (auto it = RecipeMap.begin(); it != RecipeMap.end(); it++)
	{
		string error;
		if (true) //Replace this with cancraft later
		{
			validRecipes.push_back(it.mpNode->mValue.second);
		}
		else
		{
			invalidRecipes.push_back(it.mpNode->mValue.second);
		}

	}
	validRecipes.insert(validRecipes.end(), validRecipes.begin(), invalidRecipes.end());


	for each (Recipe zurg in validRecipes)
	{
		if (true)
		{
			bool canShow = false;
	
			canShow = true;


			if (canShow)
			{
				UTFWin::UILayout* layout = new UTFWin::UILayout();
				layout->LoadByID(id("Recipe"));
				layout->SetParentWindow(inventoryWindow);

				layout->SetVisible(true);


				if (auto itemWindow = layout->FindWindowByID(id("crapingslot")))
				{
					auto icon = itemWindow->FindWindowByID(id("icon"));

					ResourceKey imgKey;
					if (App::Property::GetKey(zurg.mpPropList.get(), id("CraftIconImage"), imgKey))
					{
						ImagePtr img;
						if (Image::GetImage(imgKey, img))
						{
							ImageDrawable* drawable = new ImageDrawable();
							drawable->SetImage(img.get());
							icon->SetDrawable(drawable);
						}
					}

					itemWindow->SetFlag(UTFWin::WindowFlags::kWinFlagAlwaysInFront, true);
					itemWindow->FindWindowByID(id("itembutton"))->AddWinProc(new RecipeIcon(itemWindow, zurg));
					itemWindow->SetLayoutLocation((65 * (i % 6)) + (8 * ((i % 6) + 1)), div((i), 6).quot * 75);

					mapUI.push_back(itemWindow);
				}
				i++;
			}
		}

	}

}

bool FabricatorSystem::ReadRecipes()
{

	vector<uint32_t> recipeIDs;
	PropManager.GetPropertyListIDs(id("Recipes"), recipeIDs);
	App::ConsolePrintF("Reading Recipes...");

	for (int i = 0; i < 5; i++)
	{
		for each (uint32_t resID in recipeIDs)
		{
			try
			{
				Recipe res = Recipe(resID);
				App::ConsolePrintF((char*)res.CName.GetText());
				bool SecretRecip;
				if (App::Property::GetBool(res.mpPropList.get(), id("SecretRecip"), SecretRecip))
				{
					if (SecretRecip != true)
						RecipeMap.emplace(res.CraftingID, res);
				}
				else
				{
					
					RecipeMap.emplace(res.CraftingID, res);
				}
			}
			catch (std::exception except)
			{
				const char* text = except.what();
				wstring report;
				report.assign_convert(text);
				MessageBox(NULL, report.c_str(), LPCWSTR(u"Error adding recipe"), 0x00000010L);
			}
		}
	}
	return false;

}


FabricatorSystem* FabricatorSystem::sInstance;


Recipe::Recipe(uint32_t propID) {

	if (propID == 0)
	{
		CraftingID = 0;
		mToolID = 0;
		productAmount = 0;
		return;
	}

	if (PropManager.GetPropertyList(propID, id("Recipes"), mpPropList))
	{
		CraftingID = propID;
		App::Property::GetUInt32(mpPropList.get(), id("Recipe"), mToolID); //Gets .prop of the item it gives you.
		App::Property::GetText(mpPropList.get(), id("CName"), CName);
		App::Property::GetText(mpPropList.get(), id("CDesc"), CDesc);
		if (!App::Property::GetUInt32(mpPropList.get(), id("productAmount"), productAmount))
		{
			string error = "Research property list " + to_string(propID) + " has no output amount!";
			throw std::invalid_argument(error.c_str());
		}

		size_t count;
		uint32_t* ids;
		App::Property::GetArrayUInt32(mpPropList.get(), id("Ingredients"), count, ids);
		if (count != 0)
		{
			for (int i = 0; i < count; i++)
			{
				Ingredients.push_back(ids[i]);
			}
		}

		App::Property::GetArrayUInt32(mpPropList.get(), id("IngCount"), count, ids);
		if (count != 0)
		{
			for (int i = 0; i < count; i++)
			{
				IngCount.push_back(ids[i]);
			}
		}

		App::Property::GetArrayUInt32(mpPropList.get(), id("Blueprint"), count, ids);
		if (count != 0)
		{
			for (int i = 0; i < count; i++)
			{
				Blueprint.push_back(ids[i]);
			}
		}

	
	}
	else
	{
		string error = "Crafting property " + to_string(propID) + " is not valid!";
		throw std::invalid_argument(error.c_str());
	}


}

Recipe::Recipe(){
	CraftingID = 0;
	mToolID = 0;
	productAmount = 0;
}

Recipe::operator bool() const
{
	return (CraftingID != 0);
}
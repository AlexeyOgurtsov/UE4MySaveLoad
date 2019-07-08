#pragma once

#include "Misc/AutomationTest.h"

#include "Math/Rotator.h"

class IMySaveLoadSystem;
class ATUActor;
class AActor;

class FSaveLoadTestBase : public FAutomationTestBase
{
public:
	~FSaveLoadTestBase();
	FSaveLoadTestBase(const FString& InName, bool const bInComplexTask);

	IMySaveLoadSystem* GetSys() const { return Sys; }
	UObject* GetSysObj() const;

	UWorld* GetWorld() const { return World; }

	// ~SaveLoad Begin
	/**
	* Name of test slot name.
	*/
	static const FString SAVE_SLOT_NAME;

	/**
	* Index of test user
	*/
	static const int32 SAVE_USER_INDEX;

	/**
	* Version of SaveLoad that cannot fail.
	*
	* @see: SaveLoad
	*/
	void SaveLoadChecked();

	/**
	* Saves into test save file, then immediately loads from it.
	* @returns: true if sucessfully saved and loaded.
	*/
	bool SaveLoad();

	/**
	* Saves into test save file.
	* @returns: true if successfully saved.
	*/
	bool Save();

	/**
	* Loads from test save file.
	* @returns: true if successfuly loaded.
	*/
	bool Load();
	// ~SaveLoad End

	// ~FAutomationTestBase Begin
	/**
	* Override of the Automation framework test function.
	* @warn: Always override MyRunTest instead 
	* @warn: Super must always be called when overriding
	* (cannot be final, because IMPLEMENT_*_AUTOMATION_TEST macros override it)
	*/
	virtual bool RunTest(const FString& Parameters) override;
	// ~FAutomationTestBase End
	
	// ~Spawn helpers Begin
	template<class ActorT>
	ActorT* Spawn(const FVector& InLocation, const FRotator& InRotation = FRotator{0,0,0})
	{
		return Cast<ActorT>(Spawn(ActorT::StaticClass(), InLocation, InRotation));
	}
	AActor* Spawn(UClass* InClass, const FVector& InLocation, const FRotator& InRotation = FRotator{0,0,0});
	ATUActor* SpawnTU(const FVector& InLocation, const FRotator& InRotation = FRotator{0,0,0});
	// ~Spawn helpers End

protected:
	/**
	* My version of RunTest.
	* @warn: Override this version, and NOT RunTest itself!
	* @warn: Super should NOT be called when overriding.
	*/
	virtual bool MyRunTest(const FString& Parameters) = 0;


	/**
	* Prepares the given world (create actors etc.)
	*
	* @note: super should NOT be called when overriding.
	*/
	virtual bool PrepareWorld(UWorld* InWorld, IMySaveLoadSystem* InSys, const FString& Parameters) = 0;

	/**
	* Action to be performed on the world between Save and Load tests.
	* @note: typically super should NOT be called.
	*/
	virtual void BetweenSaveLoad(UWorld* InWorld);

private:
	// ~SaveLoad System Begin
	/**
	* Constructs the save load system and initializes it, so the calls can be made on it.
	*
	* @warn: Automatically called by the SaveLoadTestBase framework,
	* should not be called manually.
	* @returns: true if successfully initialized.
	*/
	bool InitializeSaveLoadSystem();

	IMySaveLoadSystem* Sys = nullptr;
	// ~SaveLoad System End
	
	// ~World Begin
	/**
	* Creates UWorld and initializes it.
	* @returns: true if successfully initialized.
	*/
	bool InitializeWorld();
	bool PrepareWorldLogged(UWorld* InWorld, IMySaveLoadSystem* InSys, const FString& InParameters);
	
	UWorld* World = nullptr;
	// ~World End
};

/**
* Generates a new class SaveLoad test class declaration with MyRunTest overriden.
*/
#define IMPLEMENT_SAVELOAD_TEST_PRIVATE(TClass, TBaseClass, PrettyName, TFlags, TFile, TLine)\
	IMPLEMENT_SIMPLE_AUTOMATION_TEST_PRIVATE(FAutogenSLTestBase##TClass, TBaseClass, PrettyName, TFlags, TFile, TLine);\
	bool FAutogenSLTestBase##TClass::RunTest(const FString& Parameters)\
	{\
		return TBaseClass::RunTest(Parameters);\
	}\
	class TClass : public FAutogenSLTestBase##TClass\
	{\
	public:\
	       TClass(const FString& InName)\
	: FAutogenSLTestBase##TClass(InName)\
	{\
	}\
	protected:\
		virtual bool MyRunTest(const FString& Parameters) override;\
		virtual bool PrepareWorld(UWorld* InWorld, IMySaveLoadSystem* InSys, const FString& Parameters) override;\
	};

#define IMPLEMENT_CUSTOM_SAVELOAD_TEST(TClass, TBaseClass, PrettyName, TFlags)\
	IMPLEMENT_SIMPLE_SAVELOAD_TEST_PRIVATE(TClass, TBaseClass, PrettyName, __FILE__, __LINE__)\
	namespace\
	{\
		TClass TClass##AutomationTestInstance( TEXT(#TClass) );\
	}

#define IMPLEMENT_SAVELOAD_TEST(TClass, PrettyName, TFlags)\
	IMPLEMENT_SAVELOAD_TEST_PRIVATE(TClass, FSaveLoadTestBase, PrettyName, TFlags, __FILE__, __LINE__)\
	namespace\
	{\
		TClass TClass##AutomationTestInstance( TEXT(#TClass) );\
	}

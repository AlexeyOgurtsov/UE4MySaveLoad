#pragma once

#include "Misc/AutomationTest.h"

class IMySaveLoadSystem;

class FSaveLoadTestBase : public FAutomationTestBase
{
public:
	~FSaveLoadTestBase();
	FSaveLoadTestBase(const FString& InName, bool const bInComplexTask);

	IMySaveLoadSystem* GetSys() const { return Sys; }
	UObject* GetSysObj() const;

	UWorld* GetWorld() const { return World; }

	// ~FAutomationTestBase Begin
	/**
	* Override of the Automation framework test function.
	* @warn: Always override MyRunTest instead 
	* @warn: Super must always be called when overriding
	* (cannot be final, because IMPLEMENT_*_AUTOMATION_TEST macros override it)
	*/
	virtual bool RunTest(const FString& Parameters) override;
	// ~FAutomationTestBase End

protected:
	/**
	* My version of RunTest.
	* @warn: Override this version, and NOT RunTest itself!
	* @warn: Super must NOT be called when overriding.
	*/
	virtual bool MyRunTest(const FString& Parameters) = 0;


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

// Copyright 2015-2023 Piperift. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include <Engine/World.h>
#include <Misc/AutomationTest.h>


class FACESpec : public FAutomationSpecBase
{
public:
	FACESpec(const FString& InName, const bool bInComplexTask) : FAutomationSpecBase(InName, bInComplexTask)
	{}

protected:
	void TestNotImplemented()
	{
		AddWarning("Test not implemented.");
	}

	UWorld* GetTestWorld() const;
};


#ifndef BASE_SPEC
#	define BASE_SPEC FACESpec
#endif

#define BEGIN_TESTSPEC_PRIVATE(TClass, PrettyName, TFlags, FileName, LineNumber)                             \
	class TClass : public BASE_SPEC                                                                          \
	{                                                                                                        \
		using Super = BASE_SPEC;                                                                             \
                                                                                                             \
	public:                                                                                                  \
		TClass(const FString& InName) : Super(InName, false)                                                 \
		{                                                                                                    \
			static_assert(!!((TFlags) &EAutomationTestFlags_ApplicationContextMask),                         \
				"AutomationTest has no application flag.  It shouldn't run.  See AutomationTest.h.");        \
			static_assert(                                                                                   \
				!!(((TFlags) &EAutomationTestFlags_FilterMask) == EAutomationTestFlags::SmokeFilter) ||      \
					!!(((TFlags) &EAutomationTestFlags_FilterMask) == EAutomationTestFlags::EngineFilter) || \
					!!(((TFlags) &EAutomationTestFlags_FilterMask) ==                                        \
						EAutomationTestFlags::ProductFilter) ||                                              \
					!!(((TFlags) &EAutomationTestFlags_FilterMask) == EAutomationTestFlags::PerfFilter) ||   \
					!!(((TFlags) &EAutomationTestFlags_FilterMask) == EAutomationTestFlags::StressFilter) || \
					!!(((TFlags) &EAutomationTestFlags_FilterMask) == EAutomationTestFlags::NegativeFilter), \
				"All AutomationTests must have exactly 1 filter type specified.  See AutomationTest.h.");    \
		}                                                                                                    \
		virtual EAutomationTestFlags GetTestFlags() const override                                           \
		{                                                                                                    \
			return TFlags;                                                                                   \
		}                                                                                                    \
		using Super::GetTestSourceFileName;                                                                  \
		virtual FString GetTestSourceFileName() const override                                               \
		{                                                                                                    \
			return FileName;                                                                                 \
		}                                                                                                    \
		using Super::GetTestSourceFileLine;                                                                  \
		virtual int32 GetTestSourceFileLine() const override                                                 \
		{                                                                                                    \
			return LineNumber;                                                                               \
		}                                                                                                    \
                                                                                                             \
	protected:                                                                                               \
		virtual FString GetBeautifiedTestName() const override                                               \
		{                                                                                                    \
			return PrettyName;                                                                               \
		}                                                                                                    \
		virtual void Define() override;


#if WITH_AUTOMATION_WORKER
#	define TESTSPEC(TClass, PrettyName, TFlags)                               \
		BEGIN_TESTSPEC_PRIVATE(TClass, PrettyName, TFlags, __FILE__, __LINE__) \
		}                                                                      \
		;                                                                      \
		namespace                                                              \
		{                                                                      \
			TClass TClass##AutomationSpecInstance(TEXT(#TClass));              \
		}

#	define BEGIN_TESTSPEC(TClass, PrettyName, TFlags) \
		BEGIN_TESTSPEC_PRIVATE(TClass, PrettyName, TFlags, __FILE__, __LINE__)

#	define END_TESTSPEC(TClass)                                  \
		}                                                         \
		;                                                         \
		namespace                                                 \
		{                                                         \
			TClass TClass##AutomationSpecInstance(TEXT(#TClass)); \
		}
#endif

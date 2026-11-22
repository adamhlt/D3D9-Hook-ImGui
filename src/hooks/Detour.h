#pragma once

#include "pch.h"
#include "detours.h"

namespace Detour
{
	/**
	    @brief : RAII wrapper around a Detours transaction. The transaction is aborted
	             automatically if Commit() has not been called.
	**/
	class Transaction
	{
	public:
		Transaction()
		{
			m_active = DetourTransactionBegin() == NO_ERROR;
			if (m_active)
				DetourUpdateThread(GetCurrentThread());
		}

		~Transaction()
		{
			if (m_active)
				DetourTransactionAbort();
		}

		Transaction(const Transaction&) = delete;
		Transaction& operator=(const Transaction&) = delete;

		/** @brief : Hook `target` (updated to point to the trampoline) with `hook`. **/
		template <typename Fn>
		bool Attach(Fn& target, Fn hook)
		{
			return m_active && DetourAttach(reinterpret_cast<PVOID*>(&target), reinterpret_cast<PVOID>(hook)) == NO_ERROR;
		}

		/** @brief : Remove `hook` from `target`. **/
		template <typename Fn>
		bool Detach(Fn& target, Fn hook)
		{
			return m_active && DetourDetach(reinterpret_cast<PVOID*>(&target), reinterpret_cast<PVOID>(hook)) == NO_ERROR;
		}

		bool Commit()
		{
			if (!m_active)
				return false;
			m_active = false;
			return DetourTransactionCommit() == NO_ERROR;
		}

	private:
		bool m_active = false;
	};
}

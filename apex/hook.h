#pragma once

#include <windows.h>

/*class Hook
{
private:
	void** table;
	unsigned int index;
	void* new_function;
	void* original_function;
public:
	Hook(void* object, unsigned int i, void* function) : index(i), table(*(void***)object), new_function(function) {}

	void* ReplaceVirtual()
	{
		DWORD original_protect;

		void* function = &table[index];
		VirtualProtect(function, sizeof(function), PAGE_READWRITE, &original_protect);

		original_function = table[index];
		table[index] = new_function;

		VirtualProtect(&table[index], sizeof(function), original_protect, NULL);

		return original_function;
	}
	void RevertVirtual()
	{
		DWORD original_protect;

		void* function = &table[index];
		VirtualProtect(function, sizeof(function), PAGE_READWRITE, &original_protect);

		table[index] = original_function;

		VirtualProtect(&table[index], sizeof(function), original_protect, NULL);
	}
};*/

/*class VMTHook
{
	using CopiedVMT = std::unique_ptr<std::uintptr_t[]>;

public:
	VMTHook() = default;

	VMTHook(
		const VMTHook&) = delete;

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Move assignment constructor. </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///
	/// <param name="vmthook">  [in,out] The vmthook. </param>
	///-------------------------------------------------------------------------------------------------
	VMTHook(
		VMTHook&& vmthook) noexcept
	{
		*this = std::move(vmthook);
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Destructor. </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///-------------------------------------------------------------------------------------------------
	~VMTHook()
	{
		shutdown();
	}

	VMTHook& operator = (
		const VMTHook&) = delete;

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Move assignment operator. </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///
	/// <param name="rhs">  [in,out] The right hand side. </param>
	///
	/// <returns>   A shallow copy of this object. </returns>
	///-------------------------------------------------------------------------------------------------
	VMTHook& operator = (
		VMTHook&& rhs) noexcept
	{
		m_classPointer = rhs.m_classPointer;
		m_vtable = rhs.m_vtable;
		m_copiedVTable = std::move(rhs.m_copiedVTable);
		m_numFuncs = rhs.m_numFuncs;

		rhs.m_classPointer = nullptr;
		rhs.m_vtable = nullptr;
		rhs.m_numFuncs = 0;

		return *this;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Initializes this object. </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///
	/// <param name="class_pointer">    The class pointer. </param>
	///
	/// <returns>   True if it succeeds, false if it fails. </returns>
	///-------------------------------------------------------------------------------------------------
	bool initialize(
		void* class_pointer)
	{
		shutdown();
		if (class_pointer) {
			m_classPointer = reinterpret_cast<std::uintptr_t**>(class_pointer);
			m_vtable = *m_classPointer;
			while (reinterpret_cast<std::uintptr_t*>(m_vtable[m_numFuncs])) {
				m_numFuncs += 1;
			}
			if (m_numFuncs > 0) {
				m_copiedVTable = std::make_unique<std::uintptr_t[]>(m_numFuncs);
				//std::memcpy(m_copiedVTable.get(), m_vtable, m_numFuncs * sizeof(std::uintptr_t));

				char* src = (char*)m_vtable;
				char* dest = (char*)m_copiedVTable.get();
				for (int i = 0; i < (m_numFuncs * sizeof(std::uintptr_t)); i++)
					dest[i] = src[i];
			}
		}
		return m_copiedVTable != nullptr;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Replaces a methd address (hooking). </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///
	/// <param name="index">            Zero-based index of the. </param>
	/// <param name="hooked_function">  The hooked function. </param>
	///
	/// <returns>   A std::uintptr_t. </returns>
	///-------------------------------------------------------------------------------------------------
	std::uintptr_t replace_method_address(
		const std::size_t    index,
		const std::uintptr_t hooked_function) const
	{
		if (m_copiedVTable && m_vtable && index < m_numFuncs) {
			m_copiedVTable[index] = hooked_function;
			return m_vtable[index];
		}
		return 0;
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Gets method address at index. </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///
	/// <param name="index">    Zero-based index of the. </param>
	///
	/// <returns>   The method address. </returns>
	///-------------------------------------------------------------------------------------------------
	__forceinline
		std::uintptr_t get_method_address(
			const std::size_t index) const
	{
		return !m_vtable || index >= m_numFuncs
			? 0
			: m_vtable[index];
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Hooks member m_classPointer </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///-------------------------------------------------------------------------------------------------
	void hook() const
	{
		if (m_classPointer && m_copiedVTable) {
			*m_classPointer = m_copiedVTable.get();
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Unhooks member m_classPointer </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///-------------------------------------------------------------------------------------------------
	void unhook() const
	{
		if (m_classPointer && m_vtable) {
			*m_classPointer = m_vtable;
		}
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Shuts down this object and frees any resources it is using. </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///-------------------------------------------------------------------------------------------------
	void shutdown()
	{
		unhook();
		m_classPointer = nullptr;
		m_vtable = nullptr;
		m_numFuncs = 0;
		m_copiedVTable.reset();
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Replaces a methd address (hooking). </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///
	/// <typeparam name="T">    Generic type parameter. </typeparam>
	/// <param name="index">            Zero-based index of the. </param>
	/// <param name="hooked_function">  The hooked function address. </param>
	///
	/// <returns>  Method address at index casted as generic type parameter. </returns>
	///-------------------------------------------------------------------------------------------------

	template<typename T = std::uintptr_t>
	T replace_method(
		const std::size_t    index,
		const std::uintptr_t hooked_function)
	{
		static_assert(
			std::is_integral<T>::value || std::is_pointer<T>::value,
			"Type T has to be a (function-)pointer or an integer"
			);
		/// If the return type would be still uintptr_t, the compiler would throw an error
		/// that it is impossible to cast an uintptr_t to an uintptr_t using reinterpret_cast.
		return reinterpret_cast<T>(reinterpret_cast<void*>(replace_method_address(index, hooked_function)));
	}

	///-------------------------------------------------------------------------------------------------
	/// <summary>   Gets a method address casted as T </summary>
	///
	/// <remarks>   ReactiioN, 06.08.2018. </remarks>
	///
	/// <typeparam name="T">    Generic type parameter. </typeparam>
	/// <param name="index">    Zero-based index of the. </param>
	///
	/// <returns>   Method address at index casted as generic type parameter. </returns>
	///-------------------------------------------------------------------------------------------------
	template<typename T = std::uintptr_t>
	T get_method(
		const std::size_t index) const
	{
		static_assert(
			std::is_integral<T>::value || std::is_pointer<T>::value,
			"Type T has to be a (function-)pointer or an integer"
			);
		/// If the return type would be still uintptr_t, the compiler would throw an error
		/// that it is impossible to cast an uintptr_t to an uintptr_t using reinterpret_cast.
		return reinterpret_cast<T>(reinterpret_cast<void*>(get_method_address(index)));
	}

private:
	std::uintptr_t** m_classPointer = nullptr;
	std::uintptr_t*  m_vtable = nullptr;
	CopiedVMT        m_copiedVTable = nullptr;
	std::size_t      m_numFuncs = 0;
};*/
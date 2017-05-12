#ifndef CLASSTYPEID_H
#define CLASSTYPEID_H

namespace EntityW {
	typedef int TypeId;

	template<typename TBase>
	class ClassTypeId {
	public:

		template <typename T>
		static TypeId GetTypeId()
		{
			static const TypeId id = m_nextTypeId++;
			return id;
		}

		static TypeId GetScriptTypeId() {
			return m_nextTypeId++;
		}

	private:

		static TypeId m_nextTypeId;
	};

	template <typename TBase>
	TypeId ClassTypeId<TBase>::m_nextTypeId = 0;

}

#endif
#pragma once
#include <cstdint>
#include <vector>
#include <Structures\ZetField.hpp>

namespace zet
{
	typedef std::vector<BYTE> SerializedStructure;
	namespace sql
	{
		class ZetRecord
		{
		public:
			const GUID row_guid;
			const tm	time_mark;
			const zet::configuration::schema::StructType type;
			const zet::configuration::schema::StructDevice device_id;
			const zet::configuration::schema::StructModule module_id;
			const zet::configuration::schema::StructProcess process_id;
			const zet::configuration::schema::StructChannel channel_id;
			const uint64_t seconds;
			const uint32_t nanoseconds;
			const uint32_t ticks;
			const SerializedStructure structure;
			const zet::configuration::schema::StructValidationType validation;
		public:
			ZetRecord(const GUID row_guid,
				const tm	time_mark,
				const zet::configuration::schema::StructType type,
				const zet::configuration::schema::StructDevice device_id,
				const zet::configuration::schema::StructModule module_id,
				const zet::configuration::schema::StructProcess process_id,
				const zet::configuration::schema::StructChannel channel_id,
				const uint64_t seconds,
				const uint32_t nanoseconds,
				const uint32_t ticks,
				const SerializedStructure structure,
				const zet::configuration::schema::StructValidationType validation);

			bool operator < (const ZetRecord & val)
			{
				bool ret(false);
				if (seconds != val.seconds)
					ret = seconds < val.seconds;
				else
					ret = nanoseconds < val.nanoseconds;
				return ret;
			}
		};
	}
}


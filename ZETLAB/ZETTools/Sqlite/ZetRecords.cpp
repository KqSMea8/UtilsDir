#include "stdafx.h"
#include "ZetRecords.h"

namespace zet
{
	namespace sql
	{
		ZetRecord::ZetRecord(const GUID row_guid,
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
			const zet::configuration::schema::StructValidationType validation)
			: row_guid(row_guid)
			, time_mark(time_mark)
			, type(type)
			, device_id(device_id)
			, module_id(module_id)
			, process_id(process_id)
			, channel_id(channel_id)
			, seconds(seconds)
			, nanoseconds(nanoseconds)
			, ticks(ticks)
			, structure(structure)
			, validation(validation)
		{}
	}
}
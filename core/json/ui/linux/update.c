/***

Copyright (C) 2015, 2016 Teclib'

This file is part of Armadito core.

Armadito core is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Armadito core is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Armadito core.  If not, see <http://www.gnu.org/licenses/>.

***/

#include "../update.h"
#include <stdio.h>
#include <libarmadito.h>
//#include "libarmadito-config.h"




enum a6o_json_status update_response_cb(struct armadito *armadito, struct json_request *req, struct json_response *resp, void **request_data)
{
	enum a6o_json_status status = JSON_OK;
	
	printf("[+] Debug :: TODO (linux version) :: update_response_cb...\n");

	return status;

}

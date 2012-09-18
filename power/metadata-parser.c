/* Copyright (c) 2012, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of Code Aurora Forum, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "metadata-defs.h"

int parse_metadata(char *metadata, char **metadata_saveptr,
        char *attribute, int attribute_size, char *value, int value_size)
{
    char *attribute_string;
    char *attribute_value_delim;
    unsigned int bytes_to_copy;

    attribute_string = strtok_r(metadata, ATTRIBUTE_STRING_DELIM,
            metadata_saveptr);

    if (attribute_string == NULL)
        return METADATA_PARSING_DONE;

    attribute[0] = value[0] = '\0';

    if ((attribute_value_delim = strchr(attribute_string,
                    ATTRIBUTE_VALUE_DELIM)) != NULL) {
        bytes_to_copy = MIN((attribute_value_delim - attribute_string),
                attribute_size - 1);
        strncpy(attribute, attribute_string,
                bytes_to_copy);
        attribute[bytes_to_copy] = '\0';

        bytes_to_copy = MIN(strlen(attribute_string) - strlen(attribute) - 1,
                value_size - 1);
        strncpy(value, attribute_value_delim + 1,
                bytes_to_copy);
        value[bytes_to_copy] = '\0';
    }

    return METADATA_PARSING_CONTINUE;
}

int parse_video_metadata(char *metadata,
    struct video_encode_metadata_t *video_encode_metadata)
{
    char attribute[1024], value[1024], *saveptr;
    char *temp_metadata = metadata;
    int parsing_status;

    while ((parsing_status = parse_metadata(temp_metadata, &saveptr,
            attribute, sizeof(attribute), value, sizeof(value))) == METADATA_PARSING_CONTINUE) {
        if (strlen(attribute) == strlen("state") &&
            (strncmp(attribute, "state", strlen("state")) == 0)) {
            if (strlen(value) > 0) {
                video_encode_metadata->state = atoi(value);
            }
        }

        temp_metadata = NULL;
    }

    if (parsing_status == METADATA_PARSING_ERR)
        return -1;

    return 0;
}

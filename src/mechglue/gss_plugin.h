/*
   GSS-PROXY

   Copyright (C) 2012 Red Hat, Inc.
   Copyright (C) 2012 Simo Sorce <simo.sorce@redhat.com>

   Permission is hereby granted, free of charge, to any person obtaining a
   copy of this software and associated documentation files (the "Software"),
   to deal in the Software without restriction, including without limitation
   the rights to use, copy, modify, merge, publish, distribute, sublicense,
   and/or sell copies of the Software, and to permit persons to whom the
   Software is furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
   THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#ifndef _GSS_PLUGIN_H_
#define _GSS_PLUGIN_H_

#include "src/client/gssapi_gpm.h"

struct gpp_cred_handle {
    gssx_cred *remote;
    gss_cred_id_t local;
};

struct gpp_context_handle {
    gssx_ctx *remote;
    gss_ctx_id_t local;
};

struct gpp_name_handle {
    gssx_name *remote;
    gss_name_t local;
};

extern const gss_OID_desc gssproxy_mech_interposer;

enum gpp_behavior {
    GPP_UNINITIALIZED = 0,
    GPP_LOCAL_ONLY,
    GPP_LOCAL_FIRST,
    GPP_REMOTE_FIRST,
    GPP_REMOTE_ONLY,
};

gss_OID_set gss_mech_interposer(gss_OID mech_type);
enum gpp_behavior gpp_get_behavior(void);
bool gpp_is_special_oid(const gss_OID mech_type);
const gss_OID gpp_special_mech(const gss_OID mech_type);
gss_OID_set gpp_special_available_mechs(const gss_OID_set mechs);
uint32_t gpp_map_error(uint32_t err);
uint32_t gpp_unmap_error(uint32_t err);
uint32_t gpp_remote_to_local_ctx(uint32_t *minor, gssx_ctx **remote_ctx,
                                 gss_ctx_id_t *local_ctx);
uint32_t gpp_copy_oid(uint32_t *minor, gss_OID in, gss_OID *out);
uint32_t gpp_name_to_local(uint32_t *minor, gssx_name *name,
                           gss_OID mech_type, gss_name_t *mech_name);
uint32_t gpp_local_to_name(uint32_t *minor,
                           gss_name_t local_name, gssx_name **name);

OM_uint32 gssi_acquire_cred(OM_uint32 *minor_status,
                            const gss_name_t desired_name,
                            OM_uint32 time_req,
                            const gss_OID_set desired_mechs,
                            gss_cred_usage_t cred_usage,
                            gss_cred_id_t *output_cred_handle,
                            gss_OID_set *actual_mechs,
                            OM_uint32 *time_rec);

OM_uint32 gssi_add_cred(OM_uint32 *minor_status,
                        const gss_cred_id_t input_cred_handle,
                        const gss_name_t desired_name,
                        const gss_OID desired_mech,
                        gss_cred_usage_t cred_usage,
                        OM_uint32 initiator_time_req,
                        OM_uint32 acceptor_time_req,
                        gss_cred_id_t *output_cred_handle,
                        gss_OID_set *actual_mechs,
                        OM_uint32 *initiator_time_rec,
                        OM_uint32 *acceptor_time_rec);

OM_uint32 gssi_acquire_cred_with_password(OM_uint32 *minor_status,
                                          const gss_name_t desired_name,
                                          const gss_buffer_t password,
                                          OM_uint32 time_req,
                                          const gss_OID_set desired_mechs,
                                          gss_cred_usage_t cred_usage,
                                          gss_cred_id_t *output_cred_handle,
                                          gss_OID_set *actual_mechs,
                                          OM_uint32 *time_rec);

OM_uint32 gppint_get_def_creds(OM_uint32 *minor_status,
                               enum gpp_behavior behavior,
                               struct gpp_name_handle *name,
                               gss_cred_usage_t cred_usage,
                               struct gpp_cred_handle **cred_handle);

OM_uint32 gssi_inquire_cred(OM_uint32 *minor_status,
                            gss_cred_id_t cred_handle,
                            gss_name_t *name,
                            OM_uint32 *lifetime,
                            gss_cred_usage_t *cred_usage,
                            gss_OID_set *mechanisms);

OM_uint32 gssi_inquire_cred_by_mech(OM_uint32 *minor_status,
                                    gss_cred_id_t cred_handle,
                                    gss_OID mech_type,
                                    gss_name_t *name,
                                    OM_uint32 *initiator_lifetime,
                                    OM_uint32 *acceptor_lifetime,
                                    gss_cred_usage_t *cred_usage);

OM_uint32 gssi_inquire_cred_by_oid(OM_uint32 *minor_status,
	                           const gss_cred_id_t cred_handle,
	                           const gss_OID desired_object,
	                           gss_buffer_set_t *data_set);

OM_uint32 gssi_set_cred_option(OM_uint32 *minor_status,
                               gss_cred_id_t cred_handle,
                               const gss_OID desired_object,
                               const gss_buffer_t value);

OM_uint32 gssi_store_cred(OM_uint32 *minor_status,
                          const gss_cred_id_t input_cred_handle,
                          gss_cred_usage_t input_usage,
                          const gss_OID desired_mech,
                          OM_uint32 overwrite_cred,
                          OM_uint32 default_cred,
                          gss_OID_set *elements_stored,
                          gss_cred_usage_t *cred_usage_stored);

OM_uint32 gssi_release_cred(OM_uint32 *minor_status,
                            gss_cred_id_t *cred_handle);

#endif /* _GSS_PLUGIN_H_ */
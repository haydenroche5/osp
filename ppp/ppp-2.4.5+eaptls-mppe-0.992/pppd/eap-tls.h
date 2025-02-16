/*
 * eap-tls.h
 *
 * Copyright (c) Beniamino Galvani 2005 All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The name(s) of the authors of this software must not be used to
 *    endorse or promote products derived from this software without
 *    prior written permission.
 *
 * THE AUTHORS OF THIS SOFTWARE DISCLAIM ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY
 * SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 */

#ifndef __EAP_TLS_H__
#define __EAP_TLS_H__

#include "eap.h"

#include <cyassl/ssl.h>

#define EAP_TLS_FLAGS_LI	128	/* length included flag */
#define EAP_TLS_FLAGS_MF	64	/* more fragments flag */
#define EAP_TLS_FLAGS_START	32	/* start flag */

#define EAP_TLS_MAX_LEN		65536	/* max eap tls packet size */

#define EAP_TLS_NONE_STATE	0
#define EAP_TLS_READ_STATE	1
#define EAP_TLS_WRITE_STATE	2

struct eaptls_session
{
	u_char *data;		/* buffered data */
	int datalen;		/* buffered data len */
	int dataused;		/* buffered data used */
	int offset;		/* from where to send */
	int tlslen;		/* total length of tls data */
	bool frag;		/* packet is fragmented */
	CYASSL_CTX *ctx;
	CYASSL *ssl;		/* ssl connection */
	char peer[MAXWORDLEN];	/* peer name */
	char peercertfile[MAXWORDLEN];
	bool alert_sent;
	u_char alert_sent_desc;
	bool alert_recv;
	u_char alert_recv_desc;
	char rtx[65536];	/* retransmission buffer */
	int rtx_len;
	int mtu;		/* unit mtu */
	int rwstate;
};

typedef struct pw_cb_data
{
	const void *password;
	const char *prompt_info;
} PW_CB_DATA;


CYASSL_CTX *eaptls_init_ssl(int init_server, char *cacertfile,
	char *certfile, char *peer_certfile, char *privkeyfile);
int eaptls_init_ssl_server(eap_state * esp);
int eaptls_init_ssl_client(eap_state * esp);
void eaptls_free_session(struct eaptls_session *ets);

int eaptls_receive(struct eaptls_session *ets, u_char * inp, int len);
int eaptls_send(struct eaptls_session *ets, u_char ** outp);
void eaptls_retransmit(struct eaptls_session *ets, u_char ** outp);

int get_eaptls_secret(int unit, char *client, char *server,
	char *clicertfile, char *servcertfile, char *cacertfile,
	char *pkfile, int am_server);

#ifdef MPPE
#include "mppe.h"   /* MPPE_MAX_KEY_LEN */
extern u_char mppe_send_key[MPPE_MAX_KEY_LEN];
extern u_char mppe_recv_key[MPPE_MAX_KEY_LEN];
extern int mppe_keys_set;

void eaptls_gen_mppe_keys(struct eaptls_session *ets, const char *prf_label, int client);

#endif

#endif

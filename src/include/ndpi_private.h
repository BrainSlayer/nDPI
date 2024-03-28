/*
 *
 * Copyright (C) 2011-24 - ntop.org
 *
 * This file is part of nDPI, an open source deep packet inspection
 * library based on the OpenDPI and PACE technology by ipoque GmbH
 *
 * nDPI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nDPI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with nDPI.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __NDPI_PRIVATE_H__
#define __NDPI_PRIVATE_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Not sure if we still need it.. keep it for the time being */
#ifdef NDPI_LIB_COMPILATION

/* Needed to have access to HAVE_* defines */
#ifndef _NDPI_CONFIG_H_
#include "ndpi_config.h"
#define _NDPI_CONFIG_H_
#endif

/* NDPI_NODE */
typedef struct node_t {
  char *key;
  struct node_t *left, *right;
} ndpi_node;

typedef struct {
  char *string_to_match;
  ndpi_protocol_category_t protocol_category;
} ndpi_category_match;

typedef struct {
  char *string_to_match;
  u_int16_t protocol_id;
} ndpi_tls_cert_name_match;

struct call_function_struct {
  NDPI_PROTOCOL_BITMASK detection_bitmask;
  NDPI_PROTOCOL_BITMASK excluded_protocol_bitmask;
  void (*func) (struct ndpi_detection_module_struct *, struct ndpi_flow_struct *flow);
  NDPI_SELECTION_BITMASK_PROTOCOL_SIZE ndpi_selection_bitmask;
  u_int16_t ndpi_protocol_id;
  u_int8_t detection_feature;
};

struct subprotocol_conf_struct {
  void (*func) (struct ndpi_detection_module_struct *, char *attr, char *value, int protocol_id);
};

typedef struct default_ports_tree_node {
  ndpi_proto_defaults_t *proto;
  u_int8_t customUserProto;
  u_int16_t default_port;
} default_ports_tree_node_t;


#define LINE_EQUALS(ndpi_int_one_line_struct, string_to_compare) \
  ((ndpi_int_one_line_struct).len == strlen(string_to_compare) && \
   LINE_CMP(ndpi_int_one_line_struct, string_to_compare, strlen(string_to_compare)) == 1)

#define LINE_STARTS(ndpi_int_one_line_struct, string_to_compare) \
  ((ndpi_int_one_line_struct).len >= strlen(string_to_compare) && \
   LINE_CMP(ndpi_int_one_line_struct, string_to_compare, strlen(string_to_compare)) == 1)

#define LINE_ENDS(ndpi_int_one_line_struct, string_to_compare) \
  ((ndpi_int_one_line_struct).len >= strlen(string_to_compare) && \
   memcmp((ndpi_int_one_line_struct).ptr + \
          ((ndpi_int_one_line_struct).len - strlen(string_to_compare)), \
          string_to_compare, strlen(string_to_compare)) == 0)

#define LINE_CMP(ndpi_int_one_line_struct, string_to_compare, string_to_compare_length) \
  ((ndpi_int_one_line_struct).ptr != NULL && \
   memcmp((ndpi_int_one_line_struct).ptr, string_to_compare, string_to_compare_length) == 0)

struct ndpi_int_one_line_struct {
  const u_int8_t *ptr;
  u_int16_t len;
};

struct ndpi_packet_struct {
  const struct ndpi_iphdr *iph;
  const struct ndpi_ipv6hdr *iphv6;
  const struct ndpi_tcphdr *tcp;
  const struct ndpi_udphdr *udp;
  const u_int8_t *generic_l4_ptr;	/* is set only for non tcp-udp traffic */
  const u_int8_t *payload;

  u_int64_t current_time_ms;
  u_int64_t current_time;
/* Don't change order! */

  #define host_line_idx (1)
  #define forwarded_line_idx (2)
  #define referer_line_idx (3)
  #define content_line_idx (4)
  #define content_disposition_line_idx (5)
  #define accept_line_idx (6)
  #define authorization_line_idx (7)
  #define user_agent_line_idx (8)
  #define http_url_name_idx (9)
  #define http_encoding_idx (10)
  #define http_transfer_encoding_idx (11)
  #define http_contentlen_idx (12)
  #define http_cookie_idx (13)
  #define http_origin_idx (14)
  #define http_x_session_type_idx (15)
  #define server_line_idx (16)
  #define http_method_idx (17)
  #define http_response_idx (18)
  #define last_hdr_idx (19)

  struct ndpi_int_one_line_struct line[NDPI_MAX_PARSE_LINES_PER_PACKET];
  /* HTTP headers */
  struct ndpi_int_one_line_struct null_line;
  struct ndpi_int_one_line_struct host_line;
  struct ndpi_int_one_line_struct forwarded_line;
  struct ndpi_int_one_line_struct referer_line;
  struct ndpi_int_one_line_struct content_line;
  struct ndpi_int_one_line_struct content_disposition_line;
  struct ndpi_int_one_line_struct accept_line;
  struct ndpi_int_one_line_struct authorization_line;
  struct ndpi_int_one_line_struct user_agent_line;
  struct ndpi_int_one_line_struct http_url_name;
  struct ndpi_int_one_line_struct http_origin;
  struct ndpi_int_one_line_struct server_line;
  struct ndpi_int_one_line_struct http_method;
  struct ndpi_int_one_line_struct http_response; /* the first "word" in this pointer is the
						    response code in the packet (200, etc) */
  struct ndpi_int_one_line_struct *hdr_line;
  u_int8_t http_num_headers; /* number of found (valid) header lines in HTTP request or response */

  u_int16_t l3_packet_len;
  u_int16_t payload_packet_len;
  u_int16_t parsed_lines;
  u_int16_t empty_line_position;
  u_int8_t tcp_retransmission;

  u_int8_t packet_lines_parsed_complete:1,
    packet_direction:1, empty_line_position_set:1, http_check_content:1, pad:4;
};

typedef struct ndpi_list_struct {
  char *value;
  struct ndpi_list_struct *next;
} ndpi_list;

#ifdef HAVE_NBPF
typedef struct {
  void *tree; /* cast to nbpf_filter* */
  u_int16_t l7_protocol;
} nbpf_filter;
#endif

struct ndpi_global_context {

  /* LRU caches */

  /* NDPI_PROTOCOL_OOKLA */
  int ookla_cache_is_global;
  struct ndpi_lru_cache *ookla_global_cache;

  /* NDPI_PROTOCOL_BITTORRENT */
  struct ndpi_lru_cache *bittorrent_global_cache;

  /* NDPI_PROTOCOL_ZOOM */
  struct ndpi_lru_cache *zoom_global_cache;

  /* NDPI_PROTOCOL_STUN and subprotocols */
  struct ndpi_lru_cache *stun_global_cache;
  struct ndpi_lru_cache *stun_zoom_global_cache;

  /* NDPI_PROTOCOL_TLS and subprotocols */
  struct ndpi_lru_cache *tls_cert_global_cache;

  /* NDPI_PROTOCOL_MINING and subprotocols */
  struct ndpi_lru_cache *mining_global_cache;

  /* NDPI_PROTOCOL_MSTEAMS */
  struct ndpi_lru_cache *msteams_global_cache;
};

#define CFG_MAX_LEN	256

struct ndpi_detection_module_config_struct {
  int max_packets_to_process;
  int direction_detect_enabled;
 /* In some networks, there are some anomalous TCP flows where
    the smallest ACK packets have some kind of zero padding.
    It looks like the IP and TCP headers in those frames wrongly consider the
    0x00 Ethernet padding bytes as part of the TCP payload.
    While this kind of packets is perfectly valid per-se, in some conditions
    they might be treated by the TCP reassembler logic as (partial) overlaps,
    deceiving the classification engine.
    Add an heuristic to detect these packets and to ignore them, allowing
    correct detection/classification.
    See #1946 for other details */
  int tcp_ack_paylod_heuristic;
  /* Heuristic to detect fully encrypted sessions, i.e. flows where every bytes of
     the payload is encrypted in an attempt to “look like nothing”.
     This heuristic only analyzes the first packet of the flow.
     See: https://www.usenix.org/system/files/sec23fall-prepub-234-wu-mingshi.pdf */
  int fully_encrypted_heuristic;
  int track_payload_enabled;
  int libgcrypt_init;
  int guess_on_giveup;

  char filename_config[CFG_MAX_LEN];

  int log_level;

  /* LRU caches */

  int ookla_cache_num_entries;
  int ookla_cache_ttl;
  int ookla_cache_scope;
  int bittorrent_cache_num_entries;
  int bittorrent_cache_ttl;
  int bittorrent_cache_scope;
  int zoom_cache_num_entries;
  int zoom_cache_ttl;
  int zoom_cache_scope;
  int stun_cache_num_entries;
  int stun_cache_ttl;
  int stun_cache_scope;
  int tls_cert_cache_num_entries;
  int tls_cert_cache_ttl;
  int tls_cert_cache_scope;
  int mining_cache_num_entries;
  int mining_cache_ttl;
  int mining_cache_scope;
  int msteams_cache_num_entries;
  int msteams_cache_ttl;
  int msteams_cache_scope;
  int stun_zoom_cache_num_entries;
  int stun_zoom_cache_ttl;
  int stun_zoom_cache_scope;

  /* Protocols */

  int tls_certificate_expire_in_x_days;
  int tls_app_blocks_tracking_enabled;
  int tls_sha1_fingerprint_enabled;
  /* Limit for tls buffer size */
  int tls_buf_size_limit;
  int tls_ja3c_fingerprint_enabled;
  int tls_ja3s_fingerprint_enabled;
  int tls_ja4c_fingerprint_enabled;

  int smtp_opportunistic_tls_enabled;

  int imap_opportunistic_tls_enabled;

  int pop_opportunistic_tls_enabled;

  int ftp_opportunistic_tls_enabled;

  int stun_opportunistic_tls_enabled;

  int dns_subclassification_enabled;
  int dns_parse_response_enabled;

  int http_parse_response_enabled;

  int ookla_aggressiveness;

  NDPI_PROTOCOL_BITMASK debug_bitmask;
  NDPI_PROTOCOL_BITMASK ip_list_bitmask;

  int flow_risk_lists_enabled;
  int risk_anonymous_subscriber_list_icloudprivaterelay_enabled;
  int risk_anonymous_subscriber_list_protonvpn_enabled;
  int risk_crawler_bot_list_enabled;
};

struct ndpi_detection_module_struct {
  NDPI_PROTOCOL_BITMASK detection_bitmask;

  u_int32_t ticks_per_second;
  u_int64_t current_ts;
  u_int16_t num_tls_blocks_to_follow;
  u_int8_t skip_tls_blocks_until_change_cipher:1, finalized:1, _notused:6;
  u_int8_t tls_certificate_expire_in_x_days;

  void *user_data;
  char custom_category_labels[NUM_CUSTOM_CATEGORIES][CUSTOM_CATEGORY_LABEL_LEN];

  /* callback function buffer */
  struct call_function_struct *callback_buffer;
  struct call_function_struct *callback_buffer_tcp_no_payload;
  struct call_function_struct *callback_buffer_tcp_payload;
  struct call_function_struct *callback_buffer_udp;
  struct call_function_struct *callback_buffer_non_tcp_udp;
  u_int32_t callback_buffer_size;
  u_int32_t callback_buffer_size_tcp_no_payload;
  u_int32_t callback_buffer_size_tcp_payload;
  u_int32_t callback_buffer_size_udp;
  u_int32_t callback_buffer_size_non_tcp_udp;

  default_ports_tree_node_t *tcpRoot, *udpRoot;

#ifdef NDPI_ENABLE_DEBUG_MESSAGES
  /* debug callback, only set when debug is used */
  ndpi_debug_function_ptr ndpi_debug_printf;
  const char *ndpi_debug_print_file;
  const char *ndpi_debug_print_function;
#endif

  /* misc parameters */
  u_int32_t tcp_max_retransmission_window_size;

  /* subprotocol registration handler */
  struct subprotocol_conf_struct subprotocol_conf[NDPI_MAX_SUPPORTED_PROTOCOLS + 1];

  u_int ndpi_num_supported_protocols;
  u_int ndpi_num_custom_protocols;

  /* HTTP/DNS/HTTPS/QUIC host matching */
  ndpi_automa host_automa,                     /* Used for DNS/HTTPS */
    risky_domain_automa, tls_cert_subject_automa,
    host_risk_mask_automa, common_alpns_automa;
  /* IMPORTANT: please, whenever you add a new automa:
   * update ndpi_finalize_initialization()
   * update automa_type above
   */

  ndpi_str_hash *malicious_ja3_hashmap, *malicious_sha1_hashmap;
  spinlock_t host_automa_lock;

  ndpi_list *trusted_issuer_dn;

  /* Patricia trees */
  ndpi_patricia_tree_t *ip_risk_mask_ptree;
  ndpi_patricia_tree_t *ip_risk_mask_ptree6;
  ndpi_patricia_tree_t *ip_risk_ptree;
  ndpi_patricia_tree_t *ip_risk_ptree6;
  ndpi_patricia_tree_t *protocols_ptree;  /* IP-based protocol detection */
  ndpi_patricia_tree_t *protocols_ptree6;

  /* *** If you add a new Patricia tree, please update ptree_type above! *** */

  struct {
#ifdef USE_LEGACY_AHO_CORASICK
    ndpi_automa hostnames, hostnames_shadow;
#else
    ndpi_domain_classify *sc_hostnames, *sc_hostnames_shadow;
#endif
    void *ipAddresses, *ipAddresses_shadow; /* Patricia */
    void *ipAddresses6, *ipAddresses6_shadow; /* Patricia IPv6*/
    u_int8_t categories_loaded;
  } custom_categories;

  u_int8_t ip_version_limit;
  /* NDPI_PROTOCOL_BITTORRENT */

  struct hash_ip4p_table *bt_ht;
#ifdef NDPI_DETECTION_SUPPORT_IPV6
  struct hash_ip4p_table *bt6_ht;
#endif

  /* BT_ANNOUNCE */
  struct bt_announce *bt_ann;
  int    bt_ann_len;

  struct ndpi_global_context *g_ctx;
  struct ndpi_detection_module_config_struct cfg;

  /* NDPI_PROTOCOL_TINC */
  struct cache *tinc_cache;

  /* NDPI_PROTOCOL_OOKLA */
  struct ndpi_lru_cache *ookla_cache;

  /* NDPI_PROTOCOL_BITTORRENT */
  struct ndpi_lru_cache *bittorrent_cache;

  /* NDPI_PROTOCOL_ZOOM */
  struct ndpi_lru_cache *zoom_cache;

  /* NDPI_PROTOCOL_STUN and subprotocols */
  struct ndpi_lru_cache *stun_cache;
  struct ndpi_lru_cache *stun_zoom_cache;

  /* NDPI_PROTOCOL_TLS and subprotocols */
  struct ndpi_lru_cache *tls_cert_cache;

  /* NDPI_PROTOCOL_MINING and subprotocols */
  struct ndpi_lru_cache *mining_cache;

  /* NDPI_PROTOCOL_MSTEAMS */
  struct ndpi_lru_cache *msteams_cache;

  /* *** If you add a new LRU cache, please update lru_cache_type above! *** */

  u_int16_t ndpi_to_user_proto_id[NDPI_MAX_NUM_CUSTOM_PROTOCOLS]; /* custom protocolId mapping */
  ndpi_proto_defaults_t proto_defaults[NDPI_MAX_SUPPORTED_PROTOCOLS+NDPI_MAX_NUM_CUSTOM_PROTOCOLS];

#ifdef CUSTOM_NDPI_PROTOCOLS
  #include "../../../nDPI-custom/custom_ndpi_typedefs.h"
#endif

#ifndef __KERNEL__
#ifdef HAVE_MAXMINDDB
  /* GeoIP */
  void *mmdb_city, *mmdb_as;
  u_int8_t mmdb_city_loaded, mmdb_as_loaded;
#endif
#endif

#ifndef __KERNEL__
  /* Current packet */
  struct ndpi_packet_struct packet;
#else
  struct ndpi_packet_struct packet[NR_CPUS];
#endif
  const struct ndpi_flow_input_info *input_info;

#ifdef HAVE_NBPF
  u_int8_t num_nbpf_custom_proto;
  nbpf_filter nbpf_custom_proto[MAX_NBPF_CUSTOM_PROTO];
#endif

  u_int16_t max_payload_track_len;

  ndpi_domain_classify *public_domain_suffixes;
};

#ifndef __KERNEL__
  struct ndpi_packet_struct *
  ndpi_get_packet_struct(struct ndpi_detection_module_struct *ndpi_mod);
#else
  static inline struct ndpi_packet_struct *
  ndpi_get_packet_struct(struct ndpi_detection_module_struct *ndpi_mod) {
        return &ndpi_mod->packet[smp_processor_id()];
  }
#endif

/* Used by ndpi_set_proto_subprotocols */
#define NDPI_PROTOCOL_NO_MORE_SUBPROTOCOLS (-1)
#define NDPI_PROTOCOL_MATCHED_BY_CONTENT (-2)

#define NDPI_HOSTNAME_NORM_LC 1
#define NDPI_HOSTNAME_NORM_REPLACE_IC 2
#define NDPI_HOSTNAME_NORM_STRIP_EOLSP 4
#define NDPI_HOSTNAME_NORM_ALL (NDPI_HOSTNAME_NORM_LC | NDPI_HOSTNAME_NORM_REPLACE_IC | NDPI_HOSTNAME_NORM_STRIP_EOLSP)



/* Generic */

char *strptime(const char *s, const char *format, struct tm *tm);

u_int8_t iph_is_valid_and_not_fragmented(const struct ndpi_iphdr *iph, const u_int16_t ipsize);

int current_pkt_from_client_to_server(struct ndpi_detection_module_struct *ndpi_str, const struct ndpi_flow_struct *flow);
int current_pkt_from_server_to_client(struct ndpi_detection_module_struct *ndpi_str, const struct ndpi_flow_struct *flow);

int ndpi_seen_flow_beginning(const struct ndpi_flow_struct *flow);

void ndpi_set_detected_protocol(struct ndpi_detection_module_struct *ndpi_struct,
				struct ndpi_flow_struct *flow,
				u_int16_t upper_detected_protocol,
				u_int16_t lower_detected_protocol,
				ndpi_confidence_t confidence);

void reset_detected_protocol(struct ndpi_detection_module_struct *ndpi_struct,
			     struct ndpi_flow_struct *flow);

void ndpi_set_detected_protocol_keeping_master(struct ndpi_detection_module_struct *ndpi_str,
					       struct ndpi_flow_struct *flow,
					       u_int16_t detected_protocol,
					       ndpi_confidence_t confidence);

void change_category(struct ndpi_detection_module_struct *ndpi_struct,
		     struct ndpi_flow_struct *flow,
		     ndpi_protocol_category_t protocol_category);


char *ndpi_hostname_sni_set(struct ndpi_flow_struct *flow, const u_int8_t *value, size_t value_len, int normalize);
char *ndpi_user_agent_set(struct ndpi_flow_struct *flow, const u_int8_t *value, size_t value_len);

void ndpi_parse_packet_line_info(struct ndpi_detection_module_struct *ndpi_struct,
					  struct ndpi_flow_struct *flow);
void ndpi_parse_packet_line_info_any(struct ndpi_detection_module_struct *ndpi_struct,
					      struct ndpi_flow_struct *flow);
int ndpi_handle_rule(struct ndpi_detection_module_struct *ndpi_str, 
                            char *rule);
void load_common_alpns(struct ndpi_detection_module_struct *ndpi_str);
u_int8_t is_a_common_alpn(struct ndpi_detection_module_struct *ndpi_str,
			    const char *alpn_to_check, u_int alpn_to_check_len);

int64_t asn1_ber_decode_length(const unsigned char *payload, int payload_len, u_int16_t *value_len);

u_int8_t ips_match(u_int32_t src, u_int32_t dst,
		   u_int32_t net, u_int32_t num_bits);

u_int8_t ends_with(struct ndpi_detection_module_struct *ndpi_struct,
                   char *str, char *ends);

u_int ndpi_search_tcp_or_udp_raw(struct ndpi_detection_module_struct *ndpi_struct,
				 struct ndpi_flow_struct *flow,
				 u_int8_t protocol,
				 u_int32_t saddr, u_int32_t daddr);

u_int32_t ip_port_hash_funct(u_int32_t ip, u_int16_t port);

char* ndpi_intoav4(unsigned int addr, char* buf, u_int16_t bufLen);

u_int16_t icmp4_checksum(u_int8_t const * const buf, size_t len);

ndpi_risk_enum ndpi_network_risk_ptree_match(struct ndpi_detection_module_struct *ndpi_str,
					     struct in_addr *pin /* network byte order */);

#ifndef __KERNEL__
int load_protocols_file_fd(struct ndpi_detection_module_struct *ndpi_mod, FILE *fd);
int load_categories_file_fd(struct ndpi_detection_module_struct *ndpi_str, FILE *fd, void *user_data);
int load_malicious_sha1_file_fd(struct ndpi_detection_module_struct *ndpi_str, FILE *fd);
int load_malicious_ja3_file_fd(struct ndpi_detection_module_struct *ndpi_str, FILE *fd);
int load_risk_domain_file_fd(struct ndpi_detection_module_struct *ndpi_str, FILE *fd);
int load_config_file_fd(struct ndpi_detection_module_struct *ndpi_str, FILE *fd);
int load_category_file_fd(struct ndpi_detection_module_struct *ndpi_str,
			  FILE *fd, ndpi_protocol_category_t category_id);
#endif

/* TLS */
int processClientServerHello(struct ndpi_detection_module_struct *ndpi_struct,
                             struct ndpi_flow_struct *flow, uint32_t quic_version);
void processCertificateElements(struct ndpi_detection_module_struct *ndpi_struct,
                                struct ndpi_flow_struct *flow,
                                u_int16_t p_offset, u_int16_t certificate_len);
void switch_to_tls(struct ndpi_detection_module_struct *ndpi_struct,
			  struct ndpi_flow_struct *flow, int first_dtls_pkt);
int is_dtls(const u_int8_t *buf, u_int32_t buf_len, u_int32_t *block_len);
void switch_extra_dissection_to_tls(struct ndpi_detection_module_struct *ndpi_struct,
				    struct ndpi_flow_struct *flow);

/* HTTP */
void http_process_user_agent(struct ndpi_detection_module_struct *ndpi_struct,
                             struct ndpi_flow_struct *flow,
                             const u_int8_t *ua_ptr, u_int16_t ua_ptr_len);

/* OOKLA */
int ookla_search_into_cache(struct ndpi_detection_module_struct* ndpi_struct,
                            struct ndpi_flow_struct* flow);
void ookla_add_to_cache(struct ndpi_detection_module_struct *ndpi_struct,
                        struct ndpi_flow_struct *flow);

/* QUIC */
int quic_len(const uint8_t *buf, uint64_t *value);
int quic_len_buffer_still_required(uint8_t value);
int is_version_with_var_int_transport_params(uint32_t version);
int is_version_with_tls(uint32_t version);
void process_chlo(struct ndpi_detection_module_struct *ndpi_struct,
                  struct ndpi_flow_struct *flow,
                  const u_int8_t *crypto_data, uint32_t crypto_data_len);
void process_tls(struct ndpi_detection_module_struct *ndpi_struct,
                 struct ndpi_flow_struct *flow,
                 const u_int8_t *crypto_data, uint32_t crypto_data_len);
const uint8_t *get_crypto_data(struct ndpi_detection_module_struct *ndpi_struct,
                               struct ndpi_flow_struct *flow,
                               u_int8_t *clear_payload, uint32_t clear_payload_len,
                               uint64_t *crypto_data_len);

/* RTP */
int is_valid_rtp_payload_type(uint8_t type);
int is_rtp_or_rtcp(struct ndpi_detection_module_struct *ndpi_struct,
                          struct ndpi_flow_struct *flow);
u_int8_t rtp_get_stream_type(u_int8_t payloadType, ndpi_multimedia_flow_type *s_type);

/* Bittorrent */
u_int32_t make_bittorrent_host_key(struct ndpi_flow_struct *flow, int client, int offset);
u_int32_t make_bittorrent_peers_key(struct ndpi_flow_struct *flow);
int search_into_bittorrent_cache(struct ndpi_detection_module_struct *ndpi_struct,
                                 struct ndpi_flow_struct *flow);
void ndpi_bittorrent_init(struct ndpi_detection_module_struct *ndpi_struct,
		u_int32_t size,u_int32_t size6,u_int32_t tmo,int logsize);
void ndpi_bittorrent_done(struct ndpi_detection_module_struct *ndpi_struct);
int ndpi_bittorrent_gc(struct hash_ip4p_table *ht,int key,time_t now);

/* Stun */
int stun_search_into_zoom_cache(struct ndpi_detection_module_struct *ndpi_struct, struct ndpi_flow_struct *flow);

/* TPKT */
int tpkt_verify_hdr(const struct ndpi_packet_struct * const packet);

/* Mining Protocols (Ethereum, Monero, ...) */
u_int32_t mining_make_lru_cache_key(struct ndpi_flow_struct *flow);


/* Protocols init */
void init_diameter_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_afp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_armagetron_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_amqp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_bgp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_bittorrent_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_lisp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_teredo_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ciscovpn_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_citrix_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_corba_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_crossfire_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_dcerpc_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_dhcp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_dhcpv6_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_dns_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_dofus_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_dropbox_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_eaq_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_edonkey_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ftp_control_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ftp_data_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_gnutella_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_gtp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_hsrp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_guildwars_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_h323_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_halflife2_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_hots_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_http_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_iax_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_icecast_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ipp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_irc_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_jabber_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_kakaotalk_voice_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_kerberos_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_kontiki_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ldap_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_lotus_notes_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mail_imap_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mail_pop_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mail_smtp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_maplestory_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_megaco_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mgcp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mining_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mms_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_monero_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_nats_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mpegts_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mssql_tds_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mysql_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_netbios_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_netflow_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_nfs_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_noe_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_non_tcp_udp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ntp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_openvpn_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_oracle_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_postgres_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ppstream_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_pptp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_qq_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_quake_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_quic_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_radius_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rdp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_resp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rsync_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rtcp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rtmp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rtp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rtsp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_sflow_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_shoutcast_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_sip_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_imo_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_skinny_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_smb_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_snmp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_socrates_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_socks_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_spotify_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ssh_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tls_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_starcraft_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_steam_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_stun_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_syslog_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ssdp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_teamspeak_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_teamviewer_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_telegram_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_telnet_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tftp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tvuplayer_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_usenet_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_wsd_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_veohtv_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_vhua_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_viber_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_vmware_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_vnc_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_vxlan_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_warcraft3_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_whois_das_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_world_of_warcraft_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_world_of_kung_fu_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_xbox_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_xdmcp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_zattoo_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_zmq_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_stracraft_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ubntac2_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_coap_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mqtt_dissector (struct ndpi_detection_module_struct *ndpi_struct,u_int32_t *id);
void init_someip_dissector (struct ndpi_detection_module_struct *ndpi_struct,u_int32_t *id);
void init_rx_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_git_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_drda_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_bjnp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_smpp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tinc_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_fix_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_nintendo_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_csgo_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_checkmk_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_cpha_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_apple_push_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_amazon_video_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_whatsapp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ajp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_memcached_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_nest_log_sink_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ookla_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_modbus_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_capwap_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_zabbix_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_wireguard_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_dnp3_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_104_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_s7comm_dissector(struct ndpi_detection_module_struct *ndpi_struct,u_int32_t *id);
void init_websocket_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_soap_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_dnscrypt_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mongodb_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_among_us_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_hpvirtgrp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_genshin_impact_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_z3950_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_avast_securedns_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_cassandra_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ethernet_ip_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_toca_boca_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_sd_rtn_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_raknet_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_xiaomi_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mpegdash_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rsh_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ipsec_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_collectd_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_i3d_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_riotgames_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ultrasurf_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_threema_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_alicloud_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_avast_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_softether_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_activision_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_discord_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tivoconnect_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_kismet_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_fastcgi_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_natpmp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_syncthing_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_crynet_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_line_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_munin_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_elasticsearch_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tuya_lp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tplink_shp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_merakicloud_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tailscale_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_source_engine_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_bacnet_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_oicq_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_epicgames_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_bitcoin_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_apache_thrift_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_slp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_http2_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_haproxy_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rmcp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_can_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_protobuf_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ethereum_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ptpv2_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_hart_ip_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_rtps_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_opc_ua_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_fins_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ethersio_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_beckhoff_ads_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_iso9506_1_mms_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ieee_c37118_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ethersbus_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_profinet_io_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_hislip_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_uftp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_openflow_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_json_rpc_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_kafka_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_nomachine_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_iec62056_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_hl7_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_ceph_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_roughtime_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_kcp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_valve_sdr_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_mumble_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_zoom_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_yojimbo_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_stomp_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_radmin_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_raft_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_cip_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_gearman_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_tencent_games_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_gaijin_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);
void init_c1222_dissector(struct ndpi_detection_module_struct *ndpi_struct, u_int32_t *id);

#endif

#ifdef __cplusplus
}
#endif

#endif

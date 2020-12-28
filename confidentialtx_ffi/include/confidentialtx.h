#ifndef CONFIDENTIAL_TX_FFI_H_
#define CONFIDENTIAL_TX_FFI_H_

# ifdef __cplusplus
extern "C" {
# endif


#include<stdint.h>

typedef struct ExternError {
    int32_t code;
    char *message; // error message
} ConfidentialTxError;

typedef struct ByteBuffer {
    // Note: This should never be negative, but values above
    // INT64_MAX / i64::MAX are not allowed.
    int64_t len;
    uint8_t *data;
} ResultBuffer;

/** Create a confidential tx.
 *
 *  In:     tx_json:    a pointer to tx json string.
 *  In:     tx_len:     the size of tx json string.
 *  Out:    error:      output a ExternError. 'code' 0 represents success, all other values represent failure.
 *                      If the `code` field is nonzero, there should always be a message, and if it's zero, the message will always be null.
 * 
 *  Returns: if success, return pointer and len of tx. Otherwise return null.  
 */
ResultBuffer create_confidential_tx(const uint8_t *tx_json, int32_t tx_len, ConfidentialTxError *error);

/** Create a confidential tx.
 *
 *  In:     tx_json:    a pointer to tx rlp stream.
 *  In:     tx_len:     the size of tx rlp stream.
 *  Out:    error:      output a ExternError. 'code' 0 represents success, all other values represent failure.
 *                      If the `code` field is nonzero, there should always be a message, and if it's zero, the message will always be null.
 * 
 *  Returns: if success, return pointer and len of tx. Otherwise return null.  
 */
ResultBuffer create_confidential_tx_by_rlp(const uint8_t *tx_json, int32_t tx_len, ConfidentialTxError *error);


/** Verify the confidential tx.
 *
 *  In:     tx_data:    a pointer to tx string generated by 'create_confidential_tx'.
 *  In:     tx_len:     the size of tx string.
 *  Out:    error:      output a ExternError. 'code' 0 represents success, all other values represent failure.
 *                      If the `code` field is nonzero, there should always be a message, and if it's zero, the message will always be null.
 * 
 *  Returns: if success, return pointer and len of tx log. Otherwise return null.
 */
ResultBuffer confidential_tx_verify(const uint8_t *tx_data, int32_t tx_len, ConfidentialTxError *error);

/** Create a key pair for view key and spend key.
 *
 *  Note: if sk_data is null or sk_len is 0, 'create_keypair' will output a random key pair.
 * 
 *  In:     sk_data:    a pointer to sk string of "user's input".
 *  In:     sk_len:     the size of sk string.
 *  Out:    error:      output a ExternError. 'code' 0 represents success, all other values represent failure.
 *                      If the `code` field is nonzero, there should always be a message, and if it's zero, the message will always be null.
 * 
 *  Returns: if success, return pointer and len of key pair json string. Otherwise return null.
 * 
 *  The output key pair format is rlp encoding.
 *  
 */
ResultBuffer create_keypair(const uint8_t *sk_data, int32_t sk_len, ConfidentialTxError *error);

/** Is the owner of note.
 *
 *  In:     ephemeral_pk_data:    a pointer to ephemeral_pk.
 *  In:     ephemeral_pk_len:     the size of ephemeral_pk.
 *  In:     sign_pk_data:         a pointer to sign_pk.
 *  In:     sign_pk_len:          the size of sign_pk.
 *  In:     view_sk_data:         a pointer to view_sk.
 *  In:     view_sk_len:          the size of view_sk.
 *  Out:    error:                output a ExternError. 'code' 0 represents success, it is the correct owner. All other values represent failure.
 *                                If the `code` field is nonzero, there should always be a message, and if it's zero, the message will always be null.
 * 
 * 
 */
void is_note_owner(const uint8_t *ephemeral_pk_data, int32_t ephemeral_pk_len, 
                   const uint8_t *sign_pk_data, int32_t sign_pk_len,
                   const uint8_t *spend_pk_data, int32_t spend_pk_len,
                   const uint8_t *view_sk_data, int32_t view_sk_len,
                   ConfidentialTxError *error);

/** Decrypt the quantity and blinding.
 *
 *  In:     cipher_data:    a pointer to ephemeral_pk.
 *  In:     cipher_len:     the size of ephemeral_pk.
 *  In:     view_sk_data:   a pointer to view_sk.
 *  In:     view_sk_len:    the size of view_sk.
 *  Out:    error:          output a ExternError. 'code' 0 represents success, all other values represent failure.
 *                          If the `code` field is nonzero, there should always be a message, and if it's zero, the message will always be null.
 * 
 *  Returns: if success, return pointer and len of quantity and blinding. Otherwise return null.
 *  Using the rlp encoding of quantity and blinding.
 * 
 */
ResultBuffer decrypt_note(const uint8_t *cipher_data, int32_t cipher_len, 
                          const uint8_t *view_sk_data, int32_t view_sk_len,
                          ConfidentialTxError *error);

// NOTE: MUST Release the result buffer manually
void confidential_tx_destroy_bytebuffer(ResultBuffer buffer);

// NOTE: MUST Release the error message manually
void confidential_tx_destroy_string(char *ptr);

# ifdef __cplusplus
}
# endif

#endif  // CONFIDENTIAL_TX_FFI_H_
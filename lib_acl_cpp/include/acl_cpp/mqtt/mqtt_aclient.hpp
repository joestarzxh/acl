#pragma once

#include "../acl_cpp_define.hpp"
#include <string>
#include "../stream/aio_socket_stream.hpp"

namespace acl {

class sslbase_conf;
class mqtt_header;
class mqtt_message;

/**
 * mqtt communication class in aio mode, used by mqtt client or mqtt server.
 */
class ACL_CPP_API mqtt_aclient : public aio_open_callback {
public:
	/**
	 * Constructor
	 * @param handle {aio_handle&}
	 * @param ssl_conf {sslbase_conf*} if not NULL, ssl will be used
	 */
	explicit mqtt_aclient(aio_handle& handle, sslbase_conf* ssl_conf = NULL);

	/**
	 * Because the subclass object was created dynamically, the method
	 * will be called when the subclass object is to be freed.
	 */
	virtual void destroy() = 0;

	/**
	 * Get the ssl conf object passed in constructor.
	 * @return {sslbase_conf*} return NULL if not set.
	 */
	sslbase_conf* get_ssl_conf() const {
		return ssl_conf_;
	}

	/**
	 * Connect the remote mqtt server, when connected with the server,
	 * the callback on_connect() will be called
	 * @param addr {const char*} the mqtt server's addr with the format
	 *  ip|port, or domain|port
	 * @param conn_timeout {int} the timeout for connecting to the server
	 * @param rw_timeout {int} the timeout read/write with the server
	 * @return bool {bool} if return false, you should call destroy() to
	 *  delete the subclass object
	 */
	bool open(const char* addr, int conn_timeout, int rw_timeout);

	/**
	 * Called when connect or accept one connection
	 * @param conn {aio_socket_stream*}
	 * @return bool {bool} if return false, you should call destroy() to
	 *  delete the subclass object
	 */
	bool open(aio_socket_stream* conn);

	/**
	 * Close the connection with the mqtt server async
	 */
	void close();

	/**
	 * Get the connection with the mqtt server
	 * @return {aio_socket_stream*} return NULL if not connected
	 */
	aio_socket_stream* get_conn() const {
		return conn_;
	}

	/**
	 * Set the remote host name to specify the SSL SNI for SSL handshake,
	 * used to connect one mqtt server as a connection client.
	 * @param host {const char*} the host name
	 * @return {mqtt_aclient&}
	 */
	mqtt_aclient& set_host(const char* host);

	mqtt_aclient& set_sni_prefix(const char* prefix);
	mqtt_aclient& set_sni_suffix(const char* suffix);

public:
	/**
	 * send one mqtt message to one mqtt peer.
	 * @param message {mqtt_message&}
	 * @return {bool} return true if sending successfully, or false if
	 *  some error happened.
	 */
	bool send(mqtt_message& message);

public:
	/**
	 * get the current dns addr when connection one mqtt server
	 * @param out {string&} store the result.
	 * @return {bool} return true if getting dns address successfully.
	 */
	bool get_ns_addr(string& out) const;

	/**
	 * get the mqtt server addr after resolving the domain's address.
	 * @param out {string&} store the result.
	 * @return {bool} return true if getting server's address successfully.
	 */
	bool get_server_addr(string& out) const;

protected:
	// the subclass should be created dynamically
	virtual ~mqtt_aclient();

	// @override dummy
	bool open_callback() { return true; }

	// @override
	bool timeout_callback();

	// @override
	void close_callback();

	// @override
	bool read_wakeup();

	// @override
	bool read_callback(char* data, int len);

protected:
	// wait for reading data from peer
	bool message_await();

	// virtual method called when resolving DNS failed.
	virtual void on_ns_failed() {}

	// virtual method called when it's timeout to connect mqtt server.
	virtual void on_connect_timeout() {}

	// virtual method called when it's failed to connect mqtt server.
	virtual void on_connect_failed() {}

	// virtual method called when reading timeout.
	virtual bool on_read_timeout() { return false; }

	// virtual method when connection was disconnected.
	virtual void on_disconnect() {};

	// should be implemented by subclass.
	virtual bool on_open() = 0;

	// subclass can implement the method to override the default method.
	virtual bool on_header(const mqtt_header&) { return true; };

	// should be implemented by subclass.
	virtual bool on_body(const mqtt_message&) = 0;

private:
	aio_handle& handle_;
	sslbase_conf* ssl_conf_;
	std::string sni_prefix_;
	std::string sni_suffix_;
	aio_socket_stream* conn_;
	int rw_timeout_;
	std::string host_;
	struct sockaddr_storage ns_addr_;
	struct sockaddr_storage serv_addr_;
	mqtt_header*  header_;
	mqtt_message* body_;

	// callbed when mqtt connection was created, which can be used
	// for client or server.
	bool open_done();

	// used for ssl communication.
	bool handle_ssl_handshake();

	// handle the data received from mqtt connection.
	int handle_data(char* data, int len);

	// called if it's ok for connecting one mqtt server.
	bool handle_connect(const ACL_ASTREAM_CTX* ctx);

	// called by aio module of acl.
	static int connect_callback(const ACL_ASTREAM_CTX* ctx);
};

} // namespace acl

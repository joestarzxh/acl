
all:
	@(cd httpd; make)
	@(cd httpd3; make)
	@(cd httpd4; make)
	@(cd redis; make)
	@(cd redis_threads; make)
	@(cd redis_channel; make)
	@(cd master_fiber; make)
	@(cd master_proxy; make)
	@(cd fiber_cpp; make)
	@(cd channel_cpp; make)
#	@(cd https_client; make)
#	@(cd https_server; make)
	@(cd mysql; make)
	@(cd tcp_server; make)
	@(cd tcp_client; make)
	@(cd fiber_tbox; make)

cl clean:
	@(cd httpd; make clean)
	@(cd httpd3; make clean)
	@(cd httpd4; make clean)
	@(cd redis; make clean)
	@(cd redis_threads; make clean)
	@(cd redis_channel; make clean)
	@(cd master_fiber; make clean)
	@(cd master_proxy; make clean)
	@(cd fiber_cpp; make clean)
	@(cd channel_cpp; make clean)
	@(cd https_client; make clean)
	@(cd https_server; make clean)
	@(cd mysql; make clean)
	@(cd tcp_server; make clean)
	@(cd tcp_client; make clean)
	@(cd fiber_tbox; make clean)

rebuild rb: clean all

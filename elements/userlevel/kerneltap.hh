#ifndef TUN_HH
#define TUN_HH

/*
 * =c
 * KernelTap(ADDR/MASK [, GATEWAY])
 * =s
 * user-level interface to /dev/tun or ethertap
 * V<devices>
 * =d
 *
 * Reads packets from and writes packets to a /dev/tun* or /dev/tap* device.
 * This allows a user-level Click to hand packets to the ordinary kernel IP
 * processing code. KernelTap will also transfer packets from the kernel IP
 * code if the kernel routing table has entries pointing at the device.
 *
 * Much like ToLinux, KernelTap produces and expects Ethernet packets, but the
 * Ethernet source and destination addresses are meaningless; only the
 * protocol usually matters.
 *
 * KernelTap allocates a /dev/tun* or tap* device (this might fail) and runs
 * ifconfig(8) to set the interface's local (i.e., kernel) address to ADDR and
 * the netmask to MASK. If a nonzero GATEWAY IP address (which must be on the
 * same network as the tun) is specified, then KernelTap tries to set up a
 * default route through that host.
 *
 * When cleaning up, KernelTap attempts to bring down the device via
 * ifconfig(8).
 *
 * =a
 * ToLinux, ifconfig(8) */

#include "element.hh"
#include "ipaddress.hh"

class KernelTap : public Element {
 public:
  KernelTap();
  ~KernelTap();
  
  const char *class_name() const	{ return "KernelTap"; }
  const char *processing() const	{ return "a/h"; }
  Bitvector forward_flow(int) const;
  Bitvector backward_flow(int) const;
  
  int configure(const Vector<String> &, ErrorHandler *);
  int initialize(ErrorHandler *);
  void uninitialize();
  KernelTap *clone() const;

  void selected(int fd);

  void push(int port, Packet *);
  void run_scheduled();

 private:
  String _dev_name;
  IPAddress _near;
  IPAddress _mask;
  IPAddress _gw;
  int _fd;

  int alloc_tun(struct in_addr near, struct in_addr far, ErrorHandler *errh);
  void dealloc_tun();
};

#endif

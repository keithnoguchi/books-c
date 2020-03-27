# SPDX-License-Identifier: GPL-2.0
CONF	:= $(PWD)/openssl.conf
OPENSSL	:= openssl
REQ	:= req -config $(CONF)
X509	:= x509
CERT	:= cacert.pem
PRIV	:= private/cakey.pem
RM	:= rm -rf

.PHONY: all show-cert clean
all: $(CERT) show-cert
$(CERT):
	$(OPENSSL) $(REQ) -x509 -newkey rsa -out $@ -outform PEM
show-cert:
	$(OPENSSL) $(X509) -in $(CERT) -text -noout
clean:
	$(RM) $(CERT) $(PRIV)

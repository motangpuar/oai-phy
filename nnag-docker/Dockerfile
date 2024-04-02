FROM registry.access.redhat.com/ubi9/ubi:latest AS ran-base

ARG TOKEN 
#ENV TOKEN=$TOKEN
ENV TOKEN="eyJhbGciOiJSUzI1NiIsInR5cCIgOiAiSldUIiwia2lkIiA6ICItNGVsY19WZE5fV3NPVVlmMkc0UXhyOEdjd0l4X0t0WFVDaXRhdExLbEx3In0.eyJleHAiOjE3MTIwNDI0OTYsImlhdCI6MTcxMjA0MTU5NiwiYXV0aF90aW1lIjoxNzEyMDQwNTcxLCJqdGkiOiI2NWI0ZTJjOS00ZjNlLTQ0MjMtYWU0Mi0xOWQzYzU5NThkYWYiLCJpc3MiOiJodHRwczovL3Nzby5yZWRoYXQuY29tL2F1dGgvcmVhbG1zL3JlZGhhdC1leHRlcm5hbCIsImF1ZCI6InJoc20tYXBpIiwic3ViIjoiZjo1MjhkNzZmZi1mNzA4LTQzZWQtOGNkNS1mZTE2ZjRmZTBjZTY6bWFyc2VsaW5vYWciLCJ0eXAiOiJCZWFyZXIiLCJhenAiOiJyaHNtLWFwaSIsInNlc3Npb25fc3RhdGUiOiIxM2M5YmU4MC00NGRmLTRiZTAtYTQ3Ny05MzI1NDI5NDllNTIiLCJyZWFsbV9hY2Nlc3MiOnsicm9sZXMiOlsicG9ydGFsX21hbmFnZV9zdWJzY3JpcHRpb25zIiwib2ZmbGluZV9hY2Nlc3MiLCJhZG1pbjpvcmc6YWxsIiwicG9ydGFsX21hbmFnZV9jYXNlcyIsInBvcnRhbF9zeXN0ZW1fbWFuYWdlbWVudCIsInBvcnRhbF9kb3dubG9hZCJdfSwic2NvcGUiOiJyb2xlcyB3ZWItb3JpZ2lucyBvZmZsaW5lX2FjY2VzcyIsInNpZCI6IjEzYzliZTgwLTQ0ZGYtNGJlMC1hNDc3LTkzMjU0Mjk0OWU1MiIsImFjY291bnRfbnVtYmVyIjoiMTA3NTIzOTIiLCJhY2NvdW50X2lkIjoiMTU5NzMyOTIiLCJ1c2VyX2lkIjoiNTUxNTgxNjEiLCJvcmdfaWQiOiIxNTk3MzI5MiIsIm5hbWUiOiJZb3NhZmF0IE1hcnNlbGlubyBBZ3VzIiwicHJlZmVycmVkX3VzZXJuYW1lIjoibWFyc2VsaW5vYWciLCJsb2NhbGUiOiJlbl91cyIsImdpdmVuX25hbWUiOiJZb3NhZmF0IE1hcnNlbGlubyIsImZhbWlseV9uYW1lIjoiQWd1cyIsImVtYWlsIjoibmlub2FndXNAcHJvdG9ubWFpbC5jb20ifQ.gkvViKarlVPSCyVm0uQlZTZySlHF1g2Yn897izNSnhlnUv3emm-PJZ_-fxWmY9gy1mgGN0s2Jj4_yEN9do4SDZCxMT9POxeAOhSRisYDfzHtRfq6hp2T9ucthmAbZaGBUC8cmcyYXqWl7g99G95VYxK_3xkKQ5b6plYehdeZFbsSDW1Gvs3kgriCSv0u0TLPAm88IykxS77E2BVpHSJrGAIZEJZJRm_CtRVIJby3Vf3NS64EF8usjoztDGIBIA3v_ZWDrgg6IgnPT3frebjUNrK6yr9PtjO_qupVysLvWx6S5VuKTubDUjOgK3_WOdCYntVBY3uG9qRzi6P2C-wWsI1yw24M1hdaodUAchLpzYGOOrzwjbDXSvcxRaYVck_ezcFEccpfn4c2-Tl20k98MXcaQ04UjAsdnWjCNYKLD6dFyixPVIilO5RRYnKT8cwxjI2q2hg18Dhh4kP7UoMiQWdN3A8o3mR1nGc6v75qB7jmT6HGN7kLzma2YUX0dKrMGzKMdH0JKhxoulqoCAva4d2fXlMUBGqeNnxuyg3SQGQHOHQGW8lxTJIlj-NwmAClFzu8FV89kCxPZPDzi9hmS9ftGPkyxmPP_yI_Gkiv3Fw5jnuXmC3txbnVrlBk_bBt9SHZDKnnRT-DxX3M77BmWD1ZVnUmAJ0pwQCwlomt6BM"

#RUN subscription-manager register --username dong881 --password abcdefg1234567 --auto-attach

RUN subscription-manager register --username=marselinoag --token=$TOKEN --auto-attach



RUN yum install -y ethtool gcc make module-init-tools kmod patch xz iproute pciutils python vim cmake unzip nano libaio net-tools  wget zip

###Install some libs to compile DPDK
RUN yum groupinstall -y "Development Tools"

#RUN yum install -y  ncurses-devel hmaccalc zlib-devel binutils-devel elfutils-libelf-devel bc libstdc++-4.8.5-28.el7_5.1.x86_64 gcc-c++ libstdc++-devel-4.8.5-28.el7_5.1.x86_64 autoconf-2.69-11.el7.noarch

WORKDIR /opt/dpdk/

RUN yum install -y ninja-build \
    python3-pip \
    gcc \
    gcc-c++ \
    kernel-devel \
    make \
    linuxptp \ 
    libconfig-devel \ 
    lapack-devel \ 
    blas-devel

RUN wget https://fast.dpdk.org/rel/dpdk-20.11.8.tar.xz && \
    pip3 install meson==0.58.2 && \
    tar -xJf dpdk-20.11.8.tar.xz && \
    cd dpdk-stable-20.11.8 && \
    meson build && \
    ninja -C build && \
    sudo ninja install -C build && \
    yes | sudo dnf install numactl && \
    yes | sudo dnf install numactl-devel

WORKDIR /opt/FH_7.2_dev/phy

COPY . .

RUN cd ./fhi_lib/lib && \
    make clean&& \
    RTE_SDK=/home/oai72/dpdk-stable-20.11.8/ XRAN_DIR=/home/oai72/FH_7.2_dev/phy/fhi_lib make XRAN_LIB_SO=1 
    


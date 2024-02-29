FROM ubuntu:14.04
MAINTAINER Kevin Smyth <ksmyth@isis.vanderbilt.edu>

# create user
RUN adduser --disabled-password --home=/jenkins --gecos "" jenkins
WORKDIR /jenkins

RUN apt-get update && sudo apt-get install -y libjson-spirit-dev wget unzip fakeroot debhelper build-essential autoconf libtool libxalan-c-dev libxerces-c-dev autotools-dev ccache libcppunit-dev ant junit ant-optional pccts libboost-dev zlib1g-dev swig openjdk-7-jdk libboost-python-dev

RUN apt-get update && sudo apt-get install -y curl

RUN curl -o /jenkins/slave.jar -f -s -S http://build.isis.vanderbilt.edu/jnlpJars/slave.jar
ENV JAVA_HOME /usr/lib/jvm/java-7-openjdk-amd64/

USER jenkins
CMD ["java", "-jar", "slave.jar", "-jnlpUrl", "http://build.isis.vanderbilt.edu/computer/UDM_ubuntu14.04/slave-agent.jnlp"]

# Don't try fancy stuff like debuginfo, which is useless on binary-only
# packages. Don't strip binary too
# Be sure buildpolicy set to do nothing
%define        __spec_install_post %{nil}
%define          debug_package %{nil}
%define        __os_install_post %{_dbpath}/brp-compress

Summary: Udm
Name: udm
Version: 3.13
Release: 1
License: Vanderbilt EULA
Group: Development/Tools
SOURCE0 : %{name}-%{version}.tar.gz
URL: http://isis.vanderbilt.edu

BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

%description
%{summary}

%prep
%setup -q

%build
# Empty section.

%install
rm -rf %{buildroot}
mkdir -p  %{buildroot}

# in builddir
cp -a * %{buildroot}


%clean
rm -rf %{buildroot}


%files
%defattr(-,root,root,-)
/*

%changelog
* Thu Nov 3 2011  Kevin Smyth <ksmyth@isis.vanderbilt.edu> 3.13-1
- First Build


EOF

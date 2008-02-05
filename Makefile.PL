use ExtUtils::MakeMaker;
use Config;
$BORLAND = 1 if $Config{'cc'} =~ /^bcc32/i;
WriteMakefile(
    'INC'	=> ($BORLAND ? '' : '-GX'),
    'DEFINE'	=> (Win32::IsWinNT() ? '' : '-DSTRICTLY_WINDOWS95'),
    'NAME'	=> 'Win32::Process',
    'VERSION_FROM' => 'Process.pm',
    'XS'	=> { 'Process.xs' => 'Process.cpp' },
    'dist'	=> { COMPRESS => 'gzip -9f', SUFFIX => 'gz' },
);

sub MY::xs_c {
    '
.xs.cpp:
	$(PERL) -I$(PERL_ARCHLIB) -I$(PERL_LIB) $(XSUBPP) $(XSPROTOARG) $(XSUBPPARGS) $*.xs >xstmp.c && $(MV) xstmp.c $*.cpp
';
}


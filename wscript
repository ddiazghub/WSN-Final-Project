# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

# def options(opt):
#     pass

# def configure(conf):
#     conf.check_nonfatal(header_name='stdint.h', define_name='HAVE_STDINT_H')

def build(bld):
    module = bld.create_ns3_module('wsn', ['core', 'lorawan', 'netanim', 'network',
                                               'propagation', 'mobility',
                                               'point-to-point', 'energy',
                                               'buildings'])
    module.source = [
        'model/people-counter.cc',
        'model/people-counter-node.cc',
        'model/people-counter-entry.cc',
        'model/location.cc',
        'helper/people-counter-helper.cc',
        'helper/people-counter-node-helper.cc'
        ]

    module_test = bld.create_ns3_module_test_library('wsn')
    module_test.source = [
        'test/wsn-test-suite.cc',
        ]
    # Tests encapsulating example programs should be listed here
    if (bld.env['ENABLE_EXAMPLES']):
        module_test.source.extend([
        #    'test/wsn-examples-test-suite.cc',
             ])

    headers = bld(features='ns3header')
    headers.module = 'wsn'
    headers.source = [
        'model/people-counter.h',
        'model/people-counter-node.h',
        'model/people-counter-entry.h',
        'model/location.h',
        'helper/people-counter-helper.h',
        'helper/people-counter-node-helper.h'
        ]

    if bld.env.ENABLE_EXAMPLES:
        bld.recurse('examples')

    # bld.ns3_python_bindings()


// Copyright 2025 NXP
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package libbt_vendor

import (
	"android/soong/android"
	"android/soong/cc"
	"github.com/google/blueprint/proptools"
	"strconv"
)

func init() {
	android.RegisterModuleType("nxp_libbt_vendor_defaults", libbtvendorDefaultsFactory)
}

func libbtvendorDefaultsFactory() android.Module {
	module := cc.DefaultsFactory()
	android.AddLoadHook(module, libbtvendorDefaults)
	return module
}

func libbtvendorDefaults(ctx android.LoadHookContext) {
	var Cflags []string
	type props struct {
		Target struct {
			Android struct {
				Cflags       []string
				Include_dirs []string
				proprietary  *bool
			}
		}
	}

	p := &props{}
	
	version := ctx.AConfig().PlatformSdkVersion().FinalOrFutureInt()
	Cflags = append(Cflags, "-DPLATFORM_SDK_VERSION="+strconv.Itoa(version))
	if version <= 32 {
		p.Target.Android.Include_dirs = append(p.Target.Android.Include_dirs, "system/bt/")
	} else {
		p.Target.Android.Include_dirs = append(p.Target.Android.Include_dirs, "packages/modules/Bluetooth/system/")
	}
	p.Target.Android.proprietary = proptools.BoolPtr(true)
	ctx.AppendProperties(p)
}

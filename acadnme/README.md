# acadnme
Acadnme is the basic Cppia host for Nme applications

Rebuilding
----------

First generate the 'src/cppia/export_classes.info' file, which describes which classes are in the Acadnme runtime and therefore do not need to be included in the cppia files.

```
nme cpp nocompile
```

Then build the AcadnmeBoot sample, which will create bin/apps/AcadnmeBoot.nme
```
cd samples/AcadnmeBoot
nme cppia installer
cd -
```

Now you can build + run the Acadnme host
```
nme cpp
```

Rebuilding
----------

Having built the host, you can test with
```
cd app/MyApp
nme cppia
```


If you build your application into a self-contained ".nme" file, and put it in the bin/apps directory, then the boot script will list it on the home screen.

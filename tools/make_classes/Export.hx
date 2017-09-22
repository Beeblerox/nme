import ImportAll;

@:build(CreateExports.init())
class Export
{
   @:native("typeof")
   @:extern inline static function typeOf(d:Dynamic):String
   {
      return untyped __js__("typeof(d)");
   }


   public static function main()
   {
      var global:Dynamic = untyped __js__("$global");
      global.nmeClassesLoaded = true;
      // String map hack
      global.__map_reserved = untyped __map_reserved;

      if (typeOf(global.nmeOnClasses) == 'function')
         global.nmeOnClasses();
   }
}

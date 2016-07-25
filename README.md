# Proyecto-SO-2016
Proyecto de Sistemas Operativos de Sincronizacion entre Procesos

##  Grafo de dependencias entre modulos

![Alt text](http://g.gravizo.com/g?
  digraph G {
    size ="8,8";
    "servicio tecnico";
    "servicio tecnico" -> supervisor [weight=8];
    cajeras -> "servicio tecnico";
    cajeras -> "mercadeo";
    supervisor -> proveedor;
    proveedor -> supervisor;
    supervisor -> guardia;
    guardia -> "servicio tecnico";
    supervisor -> reponedores;
    "servicio tecnico" -> mercadeo;
    mercadeo -> supervisor;
    guardia -> cliente;
    supervisor -> reponedor;
    cliente -> limpieza;
    cliente -> cajeras;
  }
)


     

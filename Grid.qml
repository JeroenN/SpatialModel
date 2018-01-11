import QtQuick 2.1
//import QtQuick.Window 2.3
//import QtQuick.Controls 2.0

Item
{
    property int gridSize: 25;

//Grid
//{
    //columns: 25
    //spacing: 0

   //Repeater
   //{

    //model: 250
    Rectangle
    {
        property int repeaterCount: 0
        property  color gridColor: "green";
        objectName: "gridBox";
        //color: gridColor;
        width: gridSize;
        height: gridSize
    }
    //}
//}
}

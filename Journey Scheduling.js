
function processData(input) {
    
    var inputValues=input.replace(/\n/g," ").split(" ");

    var N = parseInt(inputValues[0]);   //Number of Cities
    var M = parseInt(inputValues[1]);   //Number of possible journeys
    
    //bi-directional road between the cities with numbers X and Y
    var X = [];
    var Y = [];
    for (var i=0;i<N-1;i++) {
        X[i] = parseInt(inputValues[i*2+2]);
        Y[i] = parseInt(inputValues[i*2+3]);
    }
    
    //starting city V and the total number of cities to be visited K 
    var V = [];
    var K = [];
    for (var j=0;j<M;j++) {
        V[j] = parseInt(inputValues[j*2+N*2]);
        K[j] = parseInt(inputValues[j*2+N*2+1]);
    }

    //Convert edge list to adjacency list
    var graph = edgeToAdjList(X,Y);
    
    var singleNodes = 0;
    for (var key in graph) {
        if (graph.hasOwnProperty(key)) {
            if (graph[key].adjNodes.length == 1)       
                 singleNodes += 1;  
        }
    } 
    
    var alternativeMethod = false;    
    
    if(singleNodes/N > 0.9)    
        alternativeMethod = true;   
    
    if(alternativeMethod) {
        var distances = {};
        var initialNode = 1;
        
        var diameter = treeDiameter(graph, initialNode, 0, initialNode, distances, initialNode);
            
        var edge1 = initialNode;
        for (var key in distances[initialNode]) {
            if (distances[initialNode].hasOwnProperty(key)) {
                if (distances[initialNode][key]>distances[initialNode][edge1])       
                    edge1 = key;  
            }
        }     
        
        treeDiameter(graph, edge1, 0, edge1, distances, edge1);
        
        var edge2=edge1;
        for (var key in distances[edge1]) {
            if (distances[edge1].hasOwnProperty(key)) {
                if (distances[edge1][key]>distances[edge1][edge2])       
                    edge2 = key;  
            }
        }  
        
        treeDiameter(graph, edge2, 0, edge2, distances, edge1);
    

    
        //Calculate trip distances
        for(i=0;i<M;i++) {
            //console.log("Start of trip...");
            var initialCity = V[i];      //City of departure
            var tripsPending = K[i];     //Number of trips
            var totalDistance = 0;       //Distance accumulated during total trip
            
            
            var firstTripDistance = Math.max(distances[edge1][initialCity],distances[edge2][initialCity]);
    
            
            totalDistance = firstTripDistance + (diameter-1)*(tripsPending-1);
    
            
            console.log(totalDistance);
        }
    }
    else {
        var edgeDistances = {};
         
        //var edges = findTreeEdges(graph,edgeDistances);
        //var diameter = edgeDistances[edges[0]].maxDistance;
        
        var edges = findTreeDistances(graph,edgeDistances);
        
        var diameter = edgeDistances[edges[0]].maxDistance;      
        
        //Calculate trip distances
        for(i=0;i<M;i++) {
            //console.log("Start of trip...");
            var initialCity = V[i];      //City of departure
            var tripsPending = K[i];     //Number of trips
            var totalDistance = 0;       //Distance accumulated during total trip
            
            
            var firstTripDistance = 0;
            for(var j=0;j<edges.length;j++) {
                if(edgeDistances[edges[j]][initialCity]>firstTripDistance) {
                    firstTripDistance = edgeDistances[edges[j]][initialCity];
                }
            }
            
            totalDistance = firstTripDistance + diameter*(tripsPending-1);
    
            
            console.log(totalDistance);
            
        }        
    }
   

} 

process.stdin.resume();
process.stdin.setEncoding("ascii");
_input = "";
process.stdin.on("data", function (input) {
    _input += input;
});

process.stdin.on("end", function () {
   processData(_input);
});




function edgeToAdjList(X,Y) {
    var graph = {};
    
    //Loop through the edges list
    for (var i=0;i<X.length;i++) {
        //Check for X if node exists in graph, otherwise create it
        if (graph[X[i]]) {
            graph[X[i]].adjNodes.push(Y[i]);
        } else {
           graph[X[i]] = {}; 
           graph[X[i]].adjNodes = [Y[i]];
        }
        //Check for Y if node exists in graph, otherwise create it        
        if (graph[Y[i]]) {
            graph[Y[i]].adjNodes.push(X[i]);
        } else {
            graph[Y[i]] = {};             
            graph[Y[i]].adjNodes = [X[i]];
        }
    }
    return graph;   
}     



function bfs(adjlist, root) { 

    //Add distance and parent objects to every entry in the adjacency list
    for (var key in adjlist) {
        if (adjlist.hasOwnProperty(key)) {
            adjlist[key].distance = Infinity;       
            adjlist[key].parent = null;  
        }
    }    
    
    var Q = [];      
    
    adjlist[root].distance = 0;
    Q.push(adjlist[root]);
    
    //Calculate distances for every node in the graph versus the root node
    while(Q.length) {
        var current = Q.shift();
        for (var i=0;i<current.adjNodes.length;i++) {
            var n = adjlist[current.adjNodes[i]];
            if (!isFinite(n.distance)) {
                n.distance = current.distance + 1;
                n.parent = current;
                Q.push(n);
            }    
        }
      
        
    }  
    
    //Loop through the graph to find the nodes with further distance
    var furthestNodes = [];
    var maxDistance = 0;
    var dist = {};
    for (var key in adjlist) {
        if (adjlist.hasOwnProperty(key)) {
            dist[key] = adjlist[key].distance
            if (adjlist[key].distance > maxDistance) {
                maxDistance = adjlist[key].distance;
                furthestNodes = [key];              
            }
            else if (adjlist[key].distance == maxDistance) {
                furthestNodes.push(key);
            }
        }
    } 
    
    //Delete parent and distance properties
    for (var key in adjlist) {
        if (adjlist.hasOwnProperty(key)) {
            delete adjlist[key].distance;       
            delete adjlist[key].parent;
        }
    }  
    
    dist["furthestNodes"] = furthestNodes;
    dist["maxDistance"] = maxDistance;
        
    return dist;

};


function findTreeEdges(graph, distances) {
    
    var edges = [];
    var startingNode = 1;   //Assign node 1 as initial node
    
    //Calculate furthest edges from Node 1 as starting.point
    var initial = bfs(graph,startingNode);   
    
    var startingEdges = initial.furthestNodes   //Initial edges
    
    var E = [];      

    for(var i=0;i<Math.min(startingEdges.length,1);i++) {
        E.push(startingEdges[i]);
    }

    
    //Calculate distances for every node in the graph versus the root node
    while(E.length) {
        var current = E.shift();
        if(!distances[current]) {
            distances[current] = bfs(graph,current);
        }
        for (var i=0;i<Math.min(distances[current].furthestNodes.length,1);i++) {
            var n = distances[current].furthestNodes[i];
                if (!edges.includes(n)) {
                    edges.push(n);
                    E.push(n);
                }    
        }
            
    }  

    return edges;

};


function findTreeDistances(graph, distances) {
    var startingNode = 1;   //Assign node 1 as initial node
    
    var edges = [];
    
    //Calculate furthest edges from Node 1 as starting.point
    distances[startingNode] = bfs(graph,startingNode); 
 
 
    var edge1 = distances[startingNode].furthestNodes[0];  //Edge node
        
    //Calculate distances for every node in the graph versus the edge node
    distances[edge1] = bfs(graph,edge1);
    
    if(distances[startingNode].maxDistance==distances[edge1].maxDistance) {
        edges = [startingNode,edge1]
    }
    else {
        var edge2 = distances[edge1].furthestNodes[0];  //Edge node
        distances[edge2] = bfs(graph,edge2);
        edges = [edge1,edge2];
    }
    
    return edges;

};


/*The second parameter is to store the height of tree.
   Initially, we need to pass a pointer to a location with value
   as 0. So, function should be used as follows:
 
   int height = 0;
   struct node *root = SomeFunctionToMakeTree();
   int diameter = diameterOpt(root, &height); */

function diameterOpt(graph, node, previousNode, height)
{
  /* lh --> Height of left subtree
     rh --> Height of right subtree */
  var children = graph[node].adjNodes;
    console.log("Node: " + node);
    console.log("AdjNodes: " + children);
    console.log("Previous Node: " + previousNode);
    
    
  if(children.indexOf(previousNode) > -1)
      children.splice(children.indexOf(previousNode),1);
 
      console.log("Children: " + children);   
      
      
  if(!children.length)
  {
     height = 0;
     return 0; /* diameter is also 0 */
  }      
    
  var heightArr = [];
  var diameterArr = [];  
  for(var i=0;i<children.length;i++) {
      heightArr[i] = 0;
      diameterArr[i] = 0;
  } 
  

  /* Get the heights of left and right subtrees in lh and rh
    And store the returned values in ldiameter and ldiameter */
    
  for(var i=0;i<children.length;i++) {      
      diameterArr[i] = diameterOpt(graph,children[i],node,heightArr[i]);
  }   
      
  /* Height of current node is max of heights of left and
     right subtrees plus 1*/
  height = Math.max(heightArr) + 1;

  var h1,h2 = 0;
  for(var i=0;i<heightArr[i].length;i++) {
      if(heightArr[i] > h1) {
          h2 = h1;
          h1 = heightArr[i]
      }
      else if(heightArr[i] > h2) {
          h2 = heightArr[i];
      }
  }       
    
    console.log("H1: " + h1);
  console.log("H2: " + h2);
  console.log("Array of diameters: " + diameterArr);    
    
  return Math.max(h1 + h2 + 1, Math.max(diameterArr));
}



 
/* Function to get diameter of a binary tree */
function treeDiameter(tree,node,previousNode, rootNode,distances, edge)
{
    
    if(previousNode==0) {
        distances[rootNode] = {};
        distances[rootNode][rootNode] = 0;
    }
    else {
        distances[rootNode][node] = distances[rootNode][previousNode] + 1;
    }
        
    var children = JSON.parse(JSON.stringify(tree[node].adjNodes));
    

        
    if(children.indexOf(previousNode) > -1)
        children.splice(children.indexOf(previousNode),1);
  
    
    if(!children.length) {
        return 1;
    }    
    else {
        /* get the height of sub-trees */
        var subtreeHeights = [];
        for(var i=0;i<children.length;i++) {
            subtreeHeights[i] = treeHeight(tree,children[i],node);
        }
        
     
        /* get the height of sub-trees */
        var subtreeDiameters = [];
        for(var i=0;i<children.length;i++) {          
            subtreeDiameters[i] = treeDiameter(tree,children[i],node, rootNode, distances, edge);
        }    
        
        /* Return max of following three
        1) Diameter of children subtrees
        2) Height of 2 max subtree  heights + 1 */
        
        var h1 = 0;
        var h2 = 0;
        if(subtreeHeights.length<2) {
            h1 = subtreeHeights[0];
            h2 = 0;
        } 
        else {
            for(var i=0;i<subtreeHeights.length;i++) {
                if(subtreeHeights[i] > h1) {
                    h2 = h1;
                    h1 = subtreeHeights[i];
                }
                else if(subtreeHeights[i] > h2) {
                    h2 = subtreeHeights[i];
                }
            } 
        }
        
        return Math.max(h1 + h2 + 1, Math.max.apply(Math,subtreeDiameters));
        
    }    
} 
 
/* UTILITY FUNCTIONS TO TEST diameter() FUNCTION */
 
/*  The function Compute the "height" of a tree. Height is the 
    number f nodes along the longest path from the root node 
    down to the farthest leaf node.*/
function treeHeight(tree,node,previousNode)
{    
    var c = JSON.parse(JSON.stringify(tree[node].adjNodes));
    if(c.indexOf(previousNode) > -1)
        c.splice(c.indexOf(previousNode),1);
    
    if(!c.length) {
        return 1;
    } else {
        var h = [];
        for (var i=0;i<c.length;i++) {
            h[i] = treeHeight(tree,c[i],node);      
        }

            
        /* If tree is not empty then height = 1 + max of subtree heights */   
        return 1 + Math.max.apply(Math,h);
    }
} 

#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <string>

typedef std::map <std::string, std::set <std::string> > Graph;

bool BFS (Graph & graph, 
          std::string const & start, 
          std::string const & end,
          std::map<std::string, std::string> & parent)
{
  std::queue <std::string> next;
  parent.clear ();
  parent[start] = "";
  next.push(start);
  while (!next.empty()) 
  {
    std::string u = next.front();
    next.pop();
    // Here is the point where you can examine the u th vertex of graph
    // For example:
    if (u == end)
    {
	return true; 
    }
    
    for (std::set <std::string>::const_iterator iter = graph[u].begin();
         iter != graph[u].end(); 
         ++iter) 
    {
      // Look through neighbors.
      std::string v = *iter;
      if (parent.count(v) == 0) 
      {
        // If v is unvisited.
        parent[v] = u;
        next.push(v);
      }
    }
  }
  return false;
}


int  main (void)
{
  Graph g;
  char * rfolder_con[] = { "Folder" };
  char * folder_con[] = { "Application", "Packaging"};
  char * app_con[] = { "DeploymentPlan" };
  char * pkg_con[] = { "lib" };
  char * lib_con[] = { "DeploymentPlan" };

  g.insert (std::make_pair("RootFolder", 
                               std::set <std::string> (rfolder_con, rfolder_con + 1)));
  g.insert (std::make_pair("Folder", 
                               std::set <std::string> (folder_con, folder_con + 2)));
  g.insert (std::make_pair("Application", 
                               std::set <std::string> (app_con, app_con + 1)));
  g.insert (std::make_pair("Packaging", 
                               std::set <std::string> (pkg_con, pkg_con + 1)));
  g.insert (std::make_pair("DeploymentPlan", 
                               std::set <std::string> ()));
  g.insert (std::make_pair("lib", 
                               std::set <std::string> (lib_con, lib_con + 1)));

  std::map <std::string, std::string> parent;
  BFS (g, "RootFolder" , "DeploymentPlan", parent);
  std::string c = "DeploymentPlan";
  while (c != "RootFolder")
  {
    std::cout << c << std::endl;
    c = parent[c];
  }
  std::cout << c << std::endl;
  return 0;
}


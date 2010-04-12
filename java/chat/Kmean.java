
import java.util.Iterator;
import java.util.Random;
import java.util.LinkedList;
import java.util.StringTokenizer;
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.text.NumberFormat;
import java.util.Calendar;

class ClusterInfo
{
	double [] data;
	int [] clustering;
	int num_clusters;
	double [] radi;
	double goodness;
	
	
    
	private double findGoodness () 
	{
		double sum = 0;
		for (int j = 0;j < this.num_clusters; ++j)
		{
			sum += this.radi[j];
		}
		double avg_radius = sum/this.num_clusters;
		this.goodness = 1/(avg_radius * avg_radius * this.num_clusters);
		
		return this.goodness;
	}
	
	public static ClusterInfo create (
			double [] data_sample, LinkedList [] cluster_set, 
            double [] cluster_centroids, int num_clusters)
	{
		ClusterInfo info = new ClusterInfo();
		info.clustering = new int [data_sample.length];
		info.data = data_sample;
		info.num_clusters = num_clusters;
		info.radi = new double [num_clusters];
		for (int j = 0;j < num_clusters; ++j)
		{
			Iterator iter = cluster_set[j].iterator();
			double sum = 0;
			while (iter.hasNext())
			{
				double val = ((Double) iter.next()).doubleValue();
				val -= cluster_centroids[j];
				val *= val;
				sum += val; 
			}
			info.radi[j] = Math.sqrt(sum/cluster_set[j].size());
			//System.out.println ("Radi of " + j + " is " + info.radi[j]);
		}
		info.goodness = info.findGoodness();
		//System.out.println ("Avg Radi = " + info.avg_radius + " Goodness = " + info.goodness);
		for (int i = 0;i < data_sample.length; ++i)
		{
			for (int j = 0;j < num_clusters; ++j)
			{
				if (cluster_set[j].contains(new Double (data_sample[i])))
				{
					info.clustering[i] = j;
					break;
				}
			}
		}
		return info;
	}

};

public class Kmean {

	private static final int SAMPLE_SIZE = 1000;
	private static final int MAX_CLUSTERS = 10;
	private NumberFormat nf;
	static final int ROWS = 527;
        static final int COLUMNS  = 39;
        static double [][] data = new double [ROWS][COLUMNS];
	
	Kmean ()
	{
		nf = NumberFormat.getInstance();
		nf.setGroupingUsed(false);
		nf.setMaximumFractionDigits(3);
	}
	
	ClusterInfo cluster (int num_clusters, double [] data_sample)
	{
		LinkedList [] cluster_set = new LinkedList [num_clusters];
		for (int i = 0;i < num_clusters; ++i)
		{
			cluster_set[i] = new LinkedList ();
		}
		for (int i=0, j=0; i < data_sample.length; ++i, j = (++j == num_clusters) ? 0 : j)
		{
			cluster_set[j].add(new Double (data_sample[i]));
			//System.out.println(nf.format(data_sample[i]) + " is in " + j);
		}
		double [] cluster_centroids = new double [num_clusters];
		for (int i = 0;i < num_clusters; ++i)
		{
			cluster_centroids[i] = this.findCentroid (cluster_set[i]);
			//System.out.println("Centroid of " + i + " is " + nf.format(cluster_centroids[i]));
		}
		boolean swap = true;
		while (swap)
		{
			swap = false;
			for (int i = 0;i < num_clusters; ++i)
			{
				Iterator iter = cluster_set[i].iterator();
				while (iter.hasNext())
				{
					double val = ((Double) iter.next()).doubleValue();
					int closest_centroid_index =
                                          this.findClosestCentroid (val, cluster_centroids);
					//System.out.println("Closest centroid of " + val + " is " + closest_centroid_index);
					if (closest_centroid_index != i)
					{
						iter.remove();
						cluster_set[closest_centroid_index].add (new Double (val));
						cluster_centroids[i] = this.findCentroid (cluster_set[i]);
						cluster_centroids[closest_centroid_index] = 
							this.findCentroid (cluster_set[closest_centroid_index]);
						swap = true;
						/*
						System.out.println(nf.format(val) + " swapped from " + 
								           i + " to " + closest_centroid_index);
						System.out.println("Centroid of " + i + " is " + nf.format(cluster_centroids[i]));
						System.out.println("Centroid of " + closest_centroid_index + 
								" is " + nf.format(cluster_centroids[closest_centroid_index]));
						*/
					}
				}
			}
		}
		/*
		for (int j = 0;j < num_clusters; ++j)
		{
			System.out.print (j + " has: ");
			Iterator iter = cluster_set[j].iterator();
			while (iter.hasNext())
			{
				System.out.print ((Double)iter.next() + " ");
			}
			System.out.println ("");
		}
		*/

		return ClusterInfo.create (data_sample, cluster_set, cluster_centroids, num_clusters);
	}

	int findClosestCentroid (double value, double [] cluster_centroids)
	{
		double minimum = Double.MAX_VALUE;
		int index = 0;
		for (int i = 0;i < cluster_centroids.length; ++i)
		{
			if (Math.abs(value - cluster_centroids[i]) < minimum)
			{
				index = i;
				minimum = Math.abs(value - cluster_centroids[i]);
			}
		}
		return index;
	}
	
	double findCentroid (LinkedList list)
	{
		Iterator iter = list.iterator();
		double sum = 0;
		while (iter.hasNext())
		{
			sum += ((Double) iter.next()).doubleValue();
		}
		return sum/list.size();
	}
	
	int findBestCluster (double [] data_sample, ClusterInfo [] clusters)
	{
		for (int j = 0;j < MAX_CLUSTERS; ++j)
		{
			clusters[j] = this.cluster (j+2, data_sample);
		}

		double goodness = Double.MIN_VALUE;
		int good_cluster = 0;
		
		for (int j = 0;j < MAX_CLUSTERS; ++j)
		{
			if (clusters[j].goodness > goodness)
			{
				goodness = clusters[j].goodness;
				good_cluster = j;
			}
		}
		return good_cluster;
	}
	
	static double [] getData ()
	{
		double [] data_sample = new double [SAMPLE_SIZE];
		Calendar cal = Calendar.getInstance();
		int milisec = cal.get(Calendar.MILLISECOND);
		Random generator = new Random( milisec );
		for (int i = 0;i < SAMPLE_SIZE; ++i)
		{
			 data_sample[i] = generator.nextDouble() * 1000;
		}
		return data_sample;
	}
	
	public static double[] getDataFile ()
	{
	try
        {
            BufferedReader ifile = new BufferedReader(new 
                FileReader("normalized_output_spaces.txt"));
            try
            {
                String line;
                int i = 0;
                while((line = ifile.readLine())!= null)
                {
               	    if(line.trim().length() != 0)
                    {
                        StringTokenizer st = new StringTokenizer(line," ");
                        for (int j = 0;(j < COLUMNS) && (st.hasMoreTokens()); ++j)
                        {
                            data[i][j] = Double.parseDouble(st.nextToken());
                        }
                        ++i;
                        if (i >= ROWS)
                            break;
                    }
                    else
                        continue;
                }

            }
            catch(IOException ioex)
            {
                ioex.printStackTrace();
                System.err.println("IO exception");
                System.exit(1);
            }
            catch(NumberFormatException nfex)
            {
                nfex.printStackTrace();
                System.err.println(	"Value " + nfex.getMessage() +"not numeric"	);
            }
        }
        catch (FileNotFoundException fnf)
        {
  	  System.err.println("Couldn't open " + fnf.getMessage());
    	  System.exit(1);
        }

        int col = 1;
        double[] val = new double[527];
         
        for(int row=0;row<ROWS;row++)
          val[row] = data[row][col];     
                
         return val;
	}
	
	 
	
	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		double [] data_sample = getDataFile();
		ClusterInfo [] clusters = new ClusterInfo [MAX_CLUSTERS + 1];

		Kmean kmean = new Kmean ();
		
		int best_cluster = kmean.findBestCluster(data_sample, clusters);
		for (int i = 0;i < clusters[best_cluster].data.length; ++i)
		{
			System.out.println (clusters[best_cluster].data[i] + 
					            " goes in " + 
					            clusters[best_cluster].clustering[i]);			
		}
		System.out.println ("Best cluster is " + best_cluster + " as given above.");
	}
}

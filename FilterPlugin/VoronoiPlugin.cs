using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Pluto;

namespace Plugin
{
    [Category("CL.Wang")]
	[PluginType( PluginCategory.Filter )]
	public partial class VoronoiPlugin : Form, ICommonPlugin
	{
		#region コンストラクタ
		public VoronoiPlugin( )
		{
			InitializeComponent( );
		}
		#endregion

		#region フィールド
		DataManager dataManager = null;
		IPluto iPluto = null;
		#endregion

		#region IPlugin メンバ
		string IPlugin.Author { get { return ( "PLUTO Development Team" ); } }

        string IPlugin.Text { get { return ("Voronoi"); } }

        string IPlugin.Comment { get { return ("Voronoi Cluster Plugin"); } }

		bool IPlugin.Initialize( DataManager data, IPluto pluto )
		{
			// DataManager および IPluto を取得する．
			dataManager = data;
			iPluto = pluto;

			return ( true );
		}
		#endregion

		#region ICommonPlugin メンバ
		object ICommonPlugin.Run( params object[] args )
		{
			// DataManager および IPluto の取得に失敗している．または，画像が選択されていない．
			if( dataManager == null || iPluto == null || dataManager.Active == null )
			{
				return ( null );
			}

            Mist.MistArray renalData = dataManager.Active;
            int index = dataManager.IndexOf(dataManager.Active);

            if (dataManager.Count <= index + 1)
                return null;

            Mist.MistArray vesselData = dataManager[index + 1];
            if (vesselData == null)
                return null;

            Mist.MistArray voronoiData = new Mist.MistArray(renalData.Size1, renalData.Size2, renalData.Size3,
                                                           renalData.Reso1, renalData.Reso2, renalData.Reso3);
            Mist.MistArray labeledVesselData = new Mist.MistArray(renalData.Size1, renalData.Size2, renalData.Size3,
                                                           renalData.Reso1, renalData.Reso2, renalData.Reso3);
            voronoiData.Fill(0);
            labeledVesselData.Fill(0);

			// Form を表示する．
			if( this.ShowDialog( ) == DialogResult.OK )
			{
				int depth = ( int )this.numericDepth.Value;
                int kidneyValue = (int)this.numericKidneyValue.Value;
                bool manualRoot = (bool)this.checkBoxRoot.Checked;

                Vector3 clickPoint = new Vector3(0, 0, 0);
                if(manualRoot == true)
                {
                    //Get the click point.
                    iPluto.GetClickPosition(ref clickPoint);
                    //Console.Write("Click point:  {0:G}" + " {1:G}" + " {2:G}", clickPoint.X, clickPoint.Y, clickPoint.Z);
                }

				if( CppRun( renalData.Image, vesselData.Image, labeledVesselData.Image, voronoiData.Image, clickPoint.X, clickPoint.Y, clickPoint.Z, kidneyValue, depth, manualRoot ) == false )
				{
					return ( null );
				}

				// 出力画像を DataManager へ追加する．
				if( dataManager.Add( voronoiData, true, true ) < 0 )
				{
                    voronoiData.Dispose();
					return ( null );
				}
                
                if( dataManager.Add( labeledVesselData, false, true) < 0)
                {
                    labeledVesselData.Dispose();
                    return null;
                }
			}

			return ( null );
		}  
		#endregion

		#region イベント
		private void OnOK( object sender, EventArgs e )
		{
			this.DialogResult = DialogResult.OK;
		}

		private void OnCancel( object sender, EventArgs e )
		{
			this.DialogResult = DialogResult.Cancel;
		}
		#endregion

		[DllImport( "VoronoiCpp.dll", EntryPoint="Run" )]
		internal static extern bool CppRun( IntPtr renal, IntPtr vessel, IntPtr labeledVessel, IntPtr output, double x, double y, double z, int kidneyValue, int depth, bool manualRoot );
	}
}
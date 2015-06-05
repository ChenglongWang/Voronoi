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

            Mist.MistArray originImage = dataManager.Active;
            int index = dataManager.IndexOf(dataManager.Active);

            if (dataManager.Count <= index + 1)
                return null;

            Mist.MistArray maskImage = dataManager[index + 1];
            if (maskImage == null)
                return null;

            //Get the click point.
//            Vector3 clickPoint = new Vector3(0, 0, 0);
//            iPluto.GetClickPosition(ref clickPoint);
//            Console.Write("Click point:  {0:G}" + " {1:G}" + " {2:G}", clickPoint.X, clickPoint.Y, clickPoint.Z);

			// Form を表示する．
			if( this.ShowDialog( ) == DialogResult.OK )
			{
				// しきい値を設定する．
				int radius = ( int )this.numericRadius.Value;

				// しきい値処理を行う．
				if( CppRun( originImage.Image, maskImage.Image, radius ) == false )
				{
					return ( null );
				}

				// 出力画像を DataManager へ追加する．
				if( dataManager.Add( maskImage, false, true ) < 0 )
				{
					// 追加に失敗したら，出力画像のリソースを開放する．
					maskImage.Dispose( );
					return ( null );
				}
				else
				{
					return ( maskImage );
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
		internal static extern bool CppRun( IntPtr pCT, IntPtr pMK, int radius );
	}
}